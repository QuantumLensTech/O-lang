// octree.hpp — Structure Octree Octovalente (3ODS)
// Version: 1.0 Production
// Auteur: Jean-Christophe Ané
// Licence: CC BY-NC-SA 4.0
// Description: Implémentation complète d'un octree octovalent avec
//              alignement strict sur les fondations géométriques 3ODS

#pragma once

#include <array>
#include <vector>
#include <memory>
#include <cmath>
#include <cstdint>
#include <functional>
#include <limits>
#include <optional>
#include <algorithm>

namespace ods {
namespace octree {

// ═══════════════════════════════════════════════════════════════════════════
// CONSTANTES GÉOMÉTRIQUES FONDAMENTALES
// ═══════════════════════════════════════════════════════════════════════════

constexpr uint8_t NUM_OCTANTS = 8;
constexpr uint8_t NUM_PHASES = 12;
constexpr uint8_t MATRIX_SIZE = 96;  // 12×8

constexpr float SQRT_2 = 1.41421356f;  // Diagonale face
constexpr float SQRT_3 = 1.73205081f;  // Diagonale espace

// ═══════════════════════════════════════════════════════════════════════════
// STRUCTURES DE BASE
// ═══════════════════════════════════════════════════════════════════════════

/**
 * @brief Position 3D euclidienne
 */
struct Vec3 {
    float x, y, z;
    
    Vec3() : x(0), y(0), z(0) {}
    Vec3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
    
    // Opérateurs arithmétiques
    Vec3 operator+(const Vec3& other) const {
        return Vec3(x + other.x, y + other.y, z + other.z);
    }
    
    Vec3 operator-(const Vec3& other) const {
        return Vec3(x - other.x, y - other.y, z - other.z);
    }
    
    Vec3 operator*(float scalar) const {
        return Vec3(x * scalar, y * scalar, z * scalar);
    }
    
    Vec3 operator/(float scalar) const {
        return Vec3(x / scalar, y / scalar, z / scalar);
    }
    
    // Norme euclidienne
    float magnitude() const {
        return std::sqrt(x*x + y*y + z*z);
    }
    
    // Distance euclidienne
    float distance_to(const Vec3& other) const {
        return (*this - other).magnitude();
    }
    
    // Produit scalaire
    float dot(const Vec3& other) const {
        return x*other.x + y*other.y + z*other.z;
    }
};

/**
 * @brief Bounding box axis-aligned
 */
struct BoundingBox {
    Vec3 min;
    Vec3 max;
    
    BoundingBox() : min(0,0,0), max(0,0,0) {}
    BoundingBox(const Vec3& min_, const Vec3& max_) 
        : min(min_), max(max_) {}
    
    // Centre de la boîte
    Vec3 center() const {
        return (min + max) * 0.5f;
    }
    
    // Taille de la boîte
    Vec3 size() const {
        return max - min;
    }
    
    // Volume
    float volume() const {
        Vec3 s = size();
        return s.x * s.y * s.z;
    }
    
    // Test d'intersection point
    bool contains(const Vec3& point) const {
        return (point.x >= min.x && point.x <= max.x &&
                point.y >= min.y && point.y <= max.y &&
                point.z >= min.z && point.z <= max.z);
    }
    
    // Test d'intersection bbox
    bool intersects(const BoundingBox& other) const {
        return !(max.x < other.min.x || min.x > other.max.x ||
                 max.y < other.min.y || min.y > other.max.y ||
                 max.z < other.min.z || min.z > other.max.z);
    }
};

/**
 * @brief Index d'octant (0-7) avec correspondance géométrique
 */
struct OctantIndex {
    uint8_t value;  // 0-7
    
    explicit OctantIndex(uint8_t v = 0) : value(v & 7) {}
    
    // Décomposition en signes (±x, ±y, ±z)
    struct Signs {
        int8_t x, y, z;  // -1 ou +1
    };
    
    Signs to_signs() const {
        return Signs{
            static_cast<int8_t>((value & 1) ? 1 : -1),   // bit 0 → x
            static_cast<int8_t>((value & 2) ? 1 : -1),   // bit 1 → y
            static_cast<int8_t>((value & 4) ? 1 : -1)    // bit 2 → z
        };
    }
    
    // Construction depuis signes
    static OctantIndex from_signs(int8_t sx, int8_t sy, int8_t sz) {
        uint8_t v = 0;
        if (sx > 0) v |= 1;
        if (sy > 0) v |= 2;
        if (sz > 0) v |= 4;
        return OctantIndex(v);
    }
    
    // Construction depuis position
    static OctantIndex from_position(const Vec3& pos, const Vec3& center) {
        return from_signs(
            pos.x >= center.x ? 1 : -1,
            pos.y >= center.y ? 1 : -1,
            pos.z >= center.z ? 1 : -1
        );
    }
    
    // Distance de Hamming entre octants
    uint8_t hamming_distance(OctantIndex other) const {
        uint8_t xor_val = value ^ other.value;
        // Count bits set (population count)
        uint8_t count = 0;
        while (xor_val) {
            count += xor_val & 1;
            xor_val >>= 1;
        }
        return count;
    }
    
    // Distance euclidienne normalisée (1, √2, ou √3)
    float euclidean_distance(OctantIndex other) const {
        uint8_t h = hamming_distance(other);
        return std::sqrt(static_cast<float>(h));
    }
    
    // Type de connexion
    enum class ConnectionType { Edge, FaceDiagonal, SpaceDiagonal };
    
    ConnectionType connection_type(OctantIndex other) const {
        uint8_t h = hamming_distance(other);
        switch(h) {
            case 1: return ConnectionType::Edge;
            case 2: return ConnectionType::FaceDiagonal;
            case 3: return ConnectionType::SpaceDiagonal;
            default: return ConnectionType::Edge;  // Fallback (same octant)
        }
    }
};

// ═══════════════════════════════════════════════════════════════════════════
// OCTREE NODE
// ═══════════════════════════════════════════════════════════════════════════

/**
 * @brief Nœud d'octree avec données générique
 */
template<typename T>
class OctreeNode {
public:
    // Construction
    OctreeNode(const BoundingBox& bbox, uint8_t level = 0)
        : bbox_(bbox)
        , level_(level)
        , is_leaf_(true)
        , children_{}
        , data_()
    {}
    
    // Accesseurs
    const BoundingBox& bbox() const { return bbox_; }
    uint8_t level() const { return level_; }
    bool is_leaf() const { return is_leaf_; }
    bool has_children() const { return !is_leaf_; }
    
    // Données (optionnel)
    bool has_data() const { return data_.has_value(); }
    const T& data() const { return data_.value(); }
    T& data() { return data_.value(); }
    void set_data(const T& value) { data_ = value; }
    void clear_data() { data_.reset(); }
    
    // Subdivision
    void subdivide() {
        if (!is_leaf_) return;  // Déjà subdivisé
        
        Vec3 center = bbox_.center();
        Vec3 half_size = bbox_.size() * 0.5f;
        
        for (uint8_t i = 0; i < NUM_OCTANTS; ++i) {
            OctantIndex idx(i);
            auto signs = idx.to_signs();
            
            Vec3 child_min(
                signs.x > 0 ? center.x : bbox_.min.x,
                signs.y > 0 ? center.y : bbox_.min.y,
                signs.z > 0 ? center.z : bbox_.min.z
            );
            
            Vec3 child_max(
                signs.x > 0 ? bbox_.max.x : center.x,
                signs.y > 0 ? bbox_.max.y : center.y,
                signs.z > 0 ? bbox_.max.z : center.z
            );
            
            BoundingBox child_bbox(child_min, child_max);
            children_[i] = std::make_unique<OctreeNode<T>>(child_bbox, level_ + 1);
        }
        
        is_leaf_ = false;
    }
    
    // Accès enfants
    OctreeNode<T>* child(uint8_t index) {
        if (is_leaf_ || index >= NUM_OCTANTS) return nullptr;
        return children_[index].get();
    }
    
    const OctreeNode<T>* child(uint8_t index) const {
        if (is_leaf_ || index >= NUM_OCTANTS) return nullptr;
        return children_[index].get();
    }
    
    // Recherche de l'enfant contenant un point
    OctreeNode<T>* find_child_containing(const Vec3& point) {
        if (is_leaf_) return this;
        
        OctantIndex idx = OctantIndex::from_position(point, bbox_.center());
        auto* child_node = child(idx.value);
        if (!child_node) return nullptr;
        
        return child_node->find_child_containing(point);
    }
    
    // Collecte récursive
    template<typename Func>
    void visit_all(Func&& func) {
        func(this);
        if (!is_leaf_) {
            for (uint8_t i = 0; i < NUM_OCTANTS; ++i) {
                if (children_[i]) {
                    children_[i]->visit_all(std::forward<Func>(func));
                }
            }
        }
    }
    
    // Statistiques
    struct Stats {
        size_t total_nodes = 0;
        size_t leaf_nodes = 0;
        size_t internal_nodes = 0;
        size_t nodes_with_data = 0;
        uint8_t max_depth = 0;
    };
    
    Stats compute_stats() const {
        Stats stats;
        compute_stats_recursive(stats);
        return stats;
    }
    
private:
    void compute_stats_recursive(Stats& stats) const {
        stats.total_nodes++;
        if (is_leaf_) {
            stats.leaf_nodes++;
        } else {
            stats.internal_nodes++;
            for (uint8_t i = 0; i < NUM_OCTANTS; ++i) {
                if (children_[i]) {
                    children_[i]->compute_stats_recursive(stats);
                }
            }
        }
        if (has_data()) {
            stats.nodes_with_data++;
        }
        stats.max_depth = std::max(stats.max_depth, level_);
    }
    
    BoundingBox bbox_;
    uint8_t level_;
    bool is_leaf_;
    std::array<std::unique_ptr<OctreeNode<T>>, NUM_OCTANTS> children_;
    std::optional<T> data_;
};

// ═══════════════════════════════════════════════════════════════════════════
// OCTREE CONTAINER
// ═══════════════════════════════════════════════════════════════════════════

/**
 * @brief Structure Octree complète avec API de haut niveau
 */
template<typename T>
class Octree {
public:
    using Node = OctreeNode<T>;
    
    // Construction
    Octree(const BoundingBox& bbox, uint8_t max_depth = 8)
        : root_(std::make_unique<Node>(bbox))
        , max_depth_(max_depth)
    {}
    
    // Insertion de données
    void insert(const Vec3& position, const T& data) {
        if (!root_->bbox().contains(position)) {
            return;  // Hors limites
        }
        
        Node* node = find_or_create_leaf(position);
        if (node) {
            node->set_data(data);
        }
    }
    
    // Recherche de données
    std::optional<T> find(const Vec3& position) const {
        Node* node = root_->find_child_containing(position);
        if (node && node->has_data()) {
            return node->data();
        }
        return std::nullopt;
    }
    
    // Requête spatiale (bounding box)
    std::vector<T> query_bbox(const BoundingBox& query_bbox) const {
        std::vector<T> results;
        query_bbox_recursive(root_.get(), query_bbox, results);
        return results;
    }
    
    // Requête par rayon
    std::vector<T> query_radius(const Vec3& center, float radius) const {
        std::vector<T> results;
        float radius_sq = radius * radius;
        
        root_->visit_all([&](Node* node) {
            if (node->has_data()) {
                Vec3 node_center = node->bbox().center();
                float dist_sq = (node_center - center).dot(node_center - center);
                if (dist_sq <= radius_sq) {
                    results.push_back(node->data());
                }
            }
        });
        
        return results;
    }
    
    // Subdivision jusqu'à profondeur
    void subdivide_to_depth(uint8_t depth) {
        depth = std::min(depth, max_depth_);
        subdivide_recursive(root_.get(), depth);
    }
    
    // Accesseurs
    Node* root() { return root_.get(); }
    const Node* root() const { return root_.get(); }
    uint8_t max_depth() const { return max_depth_; }
    
    // Statistiques
    typename Node::Stats stats() const {
        return root_->compute_stats();
    }
    
    // Nombre total de nœuds
    size_t node_count() const {
        return stats().total_nodes;
    }
    
    // Nombre de feuilles
    size_t leaf_count() const {
        return stats().leaf_nodes;
    }
    
    // Profondeur effective
    uint8_t effective_depth() const {
        return stats().max_depth;
    }
    
    // Clear
    void clear() {
        BoundingBox bbox = root_->bbox();
        root_ = std::make_unique<Node>(bbox);
    }
    
private:
    Node* find_or_create_leaf(const Vec3& position) {
        Node* current = root_.get();
        
        while (current && current->level() < max_depth_) {
            if (current->is_leaf()) {
                current->subdivide();
            }
            
            OctantIndex idx = OctantIndex::from_position(position, current->bbox().center());
            current = current->child(idx.value);
        }
        
        return current;
    }
    
    void subdivide_recursive(Node* node, uint8_t target_depth) {
        if (!node || node->level() >= target_depth) return;
        
        if (node->is_leaf()) {
            node->subdivide();
        }
        
        for (uint8_t i = 0; i < NUM_OCTANTS; ++i) {
            Node* child = node->child(i);
            if (child) {
                subdivide_recursive(child, target_depth);
            }
        }
    }
    
    void query_bbox_recursive(const Node* node, const BoundingBox& query_bbox, 
                              std::vector<T>& results) const {
        if (!node) return;
        
        // Test intersection
        if (!node->bbox().intersects(query_bbox)) {
            return;  // Prune
        }
        
        // Si feuille avec données
        if (node->is_leaf() && node->has_data()) {
            results.push_back(node->data());
            return;
        }
        
        // Récursion sur enfants
        if (!node->is_leaf()) {
            for (uint8_t i = 0; i < NUM_OCTANTS; ++i) {
                const Node* child = node->child(i);
                if (child) {
                    query_bbox_recursive(child, query_bbox, results);
                }
            }
        }
    }
    
    std::unique_ptr<Node> root_;
    uint8_t max_depth_;
};

// ═══════════════════════════════════════════════════════════════════════════
// OCTREE AVEC MATRICE 12×8 (INTÉGRATION TEMPORELLE)
// ═══════════════════════════════════════════════════════════════════════════

/**
 * @brief Extension temporelle : Octree avec 12 phases
 */
template<typename T>
class TemporalOctree {
public:
    // 12 octrees (un par phase temporelle)
    TemporalOctree(const BoundingBox& bbox, uint8_t max_depth = 8) {
        for (uint8_t phase = 0; phase < NUM_PHASES; ++phase) {
            octrees_[phase] = std::make_unique<Octree<T>>(bbox, max_depth);
        }
    }
    
    // Insertion avec phase temporelle
    void insert(uint8_t phase, const Vec3& position, const T& data) {
        if (phase >= NUM_PHASES) return;
        octrees_[phase]->insert(position, data);
    }
    
    // Recherche avec phase temporelle
    std::optional<T> find(uint8_t phase, const Vec3& position) const {
        if (phase >= NUM_PHASES) return std::nullopt;
        return octrees_[phase]->find(position);
    }
    
    // Requête spatiale dans une phase
    std::vector<T> query_bbox(uint8_t phase, const BoundingBox& bbox) const {
        if (phase >= NUM_PHASES) return {};
        return octrees_[phase]->query_bbox(bbox);
    }
    
    // Requête spatiale sur toutes les phases
    std::vector<T> query_bbox_all_phases(const BoundingBox& bbox) const {
        std::vector<T> results;
        for (uint8_t phase = 0; phase < NUM_PHASES; ++phase) {
            auto phase_results = octrees_[phase]->query_bbox(bbox);
            results.insert(results.end(), phase_results.begin(), phase_results.end());
        }
        return results;
    }
    
    // Accès à un octree de phase
    Octree<T>* phase_octree(uint8_t phase) {
        if (phase >= NUM_PHASES) return nullptr;
        return octrees_[phase].get();
    }
    
    // Statistiques globales
    struct GlobalStats {
        size_t total_nodes = 0;
        size_t total_data = 0;
        uint8_t max_depth = 0;
    };
    
    GlobalStats global_stats() const {
        GlobalStats gstats;
        for (uint8_t phase = 0; phase < NUM_PHASES; ++phase) {
            auto stats = octrees_[phase]->stats();
            gstats.total_nodes += stats.total_nodes;
            gstats.total_data += stats.nodes_with_data;
            gstats.max_depth = std::max(gstats.max_depth, stats.max_depth);
        }
        return gstats;
    }
    
private:
    std::array<std::unique_ptr<Octree<T>>, NUM_PHASES> octrees_;
};

// ═══════════════════════════════════════════════════════════════════════════
// UTILITAIRES
// ═══════════════════════════════════════════════════════════════════════════

/**
 * @brief Calcul de la capacité théorique (formule 3ODS)
 */
inline size_t theoretical_node_count(uint8_t depth) {
    // (8^(d+1) - 1) / 7
    size_t power = 1;
    for (uint8_t i = 0; i <= depth; ++i) {
        power *= NUM_OCTANTS;
    }
    return (power - 1) / 7;
}

/**
 * @brief Calcul du nombre de feuilles à une profondeur donnée
 */
inline size_t leaf_count_at_depth(uint8_t depth) {
    // 8^depth
    size_t count = 1;
    for (uint8_t i = 0; i < depth; ++i) {
        count *= NUM_OCTANTS;
    }
    return count;
}

} // namespace octree
} // namespace ods

# O Language - Index des Fichiers

**Date** : 22 décembre 2025  
**Version** : 1.0  
**Auteur** : Jean-Christophe Ané

## 📁 Structure Complète

```
O-Language/
├── README.md                      # Documentation principale
├── FILES_INDEX.md                 # Ce fichier
│
├── include/o/                     # Headers (bibliothèque header-only)
│   ├── o.hpp                      # Point d'entrée principal (inclut tout)
│   ├── core.hpp                   # Type O<N> fondamental
│   ├── logic.hpp                  # Opérateurs logiques multi-états
│   ├── operators.hpp              # Opérateurs arithmétiques avancés
│   ├── ranges.hpp                 # Itérateurs et ranges
│   ├── functional.hpp             # Programmation fonctionnelle
│   ├── channels.hpp               # Canaux hardware (OctoBIN/QUANT/TOPO)
│   └── quantum.hpp                # Support quantique
│
└── examples/                      # Exemples d'utilisation
    └── example_basic.cpp          # Exemple basique
```

## 📋 Fichiers Headers

### 1. **o.hpp** (Header principal)
- Point d'entrée unique
- Inclut tous les autres headers
- Usage : `#include <o/o.hpp>`

### 2. **core.hpp** (835 lignes)
- Type fondamental `O<N>`
- Constructeurs, accesseurs
- Opérateurs de comparaison
- Arithmétique modulaire (modulo N)
- Incrémentation/décrémentation cyclique
- Conversions

**Types définis** :
- `O<N>` — Type universel N-états
- `O2`, `O3`, `O4`, `O8`, `O16`, etc. — Aliases
- `O_array<N, Size>` — Tableaux de valeurs N-états

### 3. **logic.hpp** (420 lignes)
- Logique de Łukasiewicz (min/max) — Par défaut
- Logique Product (multiplication)
- Logique de Gödel (négation binaire)
- Opérations sur tableaux

**Opérateurs** :
- `&` — AND (minimum)
- `|` — OR (maximum)
- `~` — NOT (complément)
- `^` — XOR (addition cyclique)
- `implies()`, `equivalent()`
- `and_product()`, `or_product()`
- `not_godel()`, `implies_godel()`

### 4. **operators.hpp** (530 lignes)
- Arithmétique modulaire avancée
- Exponentiation modulaire
- Opérations trigonométriques cycliques
- Métriques de distance
- Manipulation de bits
- Opérations d'agrégation

**Fonctions** :
- `pow_mod()` — Exponentiation modulaire
- `sqrt_mod()` — Racine carrée modulaire
- `sin_cyclic()`, `cos_cyclic()` — Trigonométrie cyclique
- `hamming_distance()`, `manhattan_distance()`, `euclidean_distance_squared()`
- `rotate_left()`, `rotate_right()`
- `sum()`, `product()`, `min()`, `max()`, `average()`

### 5. **ranges.hpp** (480 lignes)
- Itérateurs pour O<N>
- Ranges complets et partiels
- Itération avec stride
- Génération de toutes les combinaisons
- Filtrage et comptage

**Classes** :
- `O_range<N>` — Range [0, N-1]
- `O_subrange<N>` — Range [start, end)
- `O_strided_range<N>` — Itération avec pas
- `O_array_generator<N, Size>` — Toutes les combinaisons

**Fonctions** :
- `all_states<N>()` — Itérer tous les états
- `range(start, end)` — Subrange
- `stride(step)` — Itération avec pas
- `filter(predicate)` — Filtrer états
- `count_if(predicate)` — Compter états

### 6. **functional.hpp** (300 lignes)
- Programmation fonctionnelle
- Map, filter, reduce
- Composition de fonctions
- Application partielle
- Monades

**Fonctions** :
- `map(f)` — Appliquer fonction à tous états
- `filter(pred)` — Filtrer selon prédicat
- `foldl()`, `foldr()` — Fold gauche/droite
- `reduce(f)` — Réduire avec opération binaire
- `zip_with(a, b, f)` — Zip avec fonction
- `compose(f, g)` — Composition (f ∘ g)
- `curry(f)` — Curryfication

**Classes** :
- `Maybe<N>` — Monade Optional pour O<N>

### 7. **channels.hpp** (260 lignes)
- Abstraction hardware
- Canaux pour différents backends

**Classes** :
- `Channel<N>` — Interface abstraite
- `BinaryChannel<N>` — OctoBIN (émulation binaire)
- `QuantumChannel<N>` — OctoQUANT (processeurs quantiques)
- `TopologicalChannel<N>` — OctoTOPO (topologique)
- `ChannelManager<N>` — Gestionnaire de canaux

### 8. **quantum.hpp** (420 lignes)
- Support calcul quantique
- États en superposition
- Portes quantiques
- Intrication

**Classes** :
- `QuantumState<N>` — État quantique |ψ⟩
- `EntangledPair<N>` — Paires intriquées

**Fonctions** :
- `hadamard()` — Porte Hadamard
- `phase_gate()` — Rotation de phase
- `rotation()` — Rotation générale
- `quantum_not()` — NOT quantique
- `measure_computational()` — Mesure base computationnelle
- `measure_hadamard()` — Mesure base Hadamard

## 📊 Statistiques

| Fichier | Lignes | Description |
|---------|--------|-------------|
| core.hpp | 835 | Type O<N> fondamental |
| logic.hpp | 420 | Logiques multi-états |
| operators.hpp | 530 | Arithmétique avancée |
| ranges.hpp | 480 | Itérateurs |
| functional.hpp | 300 | Prog. fonctionnelle |
| channels.hpp | 260 | Abstraction hardware |
| quantum.hpp | 420 | Support quantique |
| o.hpp | 30 | Header principal |
| **TOTAL** | **3,275** | **Lignes de code** |

## 🎯 Usage

### Inclusion minimale
```cpp
#include <o/o.hpp>
using namespace o;
```

### Inclusion sélective
```cpp
#include <o/core.hpp>     // Juste le type O<N>
#include <o/logic.hpp>    // + logique
#include <o/quantum.hpp>  // + quantique
```

## 🔗 Relation avec 3ODS

**O est indépendant de 3ODS.**

- **O** = Langage universel pour calcul N-états (N quelconque)
- **3ODS** = Système d'exploitation utilisant O avec N=8

**3ODS utilise O**, pas l'inverse. O peut servir pour :
- N=2 (binaire)
- N=3 (ternaire)
- N=8 (octovalent, 3ODS)
- N=2^k (systèmes quantiques)
- Tout N

## 📄 License

CC BY-NC-SA 4.0

## 👤 Auteur

Jean-Christophe Ané  
Décembre 2025

---

*O: Where binary is just N=2*

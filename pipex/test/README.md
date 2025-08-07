# PIPEX Test Suite

Ce répertoire contient une suite complète de tests pour votre projet pipex.

## Structure des tests

### 1. Tests de base (`test_basic.sh`)
- ✅ Fonctionnalités de base avec un seul pipe
- ✅ Conformité avec le comportement du shell
- ✅ Gestion des arguments simples
- ✅ Comparaison des sorties avec les équivalents shell

### 2. Tests des bonus (`test_bonus.sh`)
- ✅ Multiple pipes (3+ commandes)
- ✅ Heredoc avec comportement d'append (`>>`)
- ✅ Gestion de commandes complexes
- ✅ Tests de performance

### 3. Tests de gestion d'erreurs (`test_errors.sh`)
- ✅ Codes de retour corrects
- ✅ Messages d'erreur appropriés
- ✅ Gestion des signaux
- ✅ Tests de fuites mémoire (si valgrind disponible)
- ✅ Gestion des descripteurs de fichiers
- ✅ Tests de stress et performance

### 4. Nouvelles fonctionnalités de vérification
- ✅ **Vérification automatique des descripteurs de fichiers après chaque test**
- ✅ **Détection de fuites mémoire avec valgrind (optionnel)**
- ✅ **Tests de stress avec vérifications robustes**
- ✅ **Logging centralisé dans un seul fichier `pipex_tests.log`**

## Comment utiliser

### Lancement rapide
```bash
cd test
chmod +x run_tests.sh
./run_tests.sh
```

### Tests individuels

#### Tests de base uniquement
```bash
chmod +x test_basic.sh
./test_basic.sh
```

#### Tests bonus uniquement
```bash
chmod +x test_bonus.sh
./test_bonus.sh
```

#### Tests de gestion d'erreurs
```bash
chmod +x test_errors.sh
./test_errors.sh
```

#### Tests avancés combinés
```bash
chmod +x run_advanced.sh
./run_advanced.sh
```

#### Nettoyage rapide
```bash
chmod +x cleanup.sh
./cleanup.sh
```

## Prérequis

### Outils nécessaires
- `bash` (version 4.0+)
- `make`
- Commandes Unix standard : `cat`, `wc`, `grep`, `sort`, `head`, `tail`, etc.

### Outils optionnels
- `valgrind` (pour les tests de fuites mémoire)
- `bc` (pour les calculs de performance)
- `lsof` (pour les tests de descripteurs de fichiers)

### Fichiers générés
- `pipex_tests.log` : Log centralisé de tous les tests
- Fichiers temporaires `test_*.txt`, `out*.txt` (supprimés automatiquement)

## Types de tests

### Tests de conformité
Les tests comparent la sortie de votre pipex avec l'équivalent shell :
```bash
# Votre pipex
./pipex infile "cmd1" "cmd2" outfile

# Équivalent shell
< infile cmd1 | cmd2 > outfile
```

**Nouveauté :** Chaque test vérifie automatiquement :
- ✅ La gestion des descripteurs de fichiers
- ✅ L'absence de fuites mémoire (avec valgrind si disponible)
- ✅ Les codes de retour appropriés

### Tests d'erreur
Vérification des codes de retour pour :
- ❌ Arguments manquants
- ❌ Fichiers inexistants
- ❌ Permissions refusées
- ❌ Commandes invalides

### Tests de performance
Mesure du temps d'exécution sur de gros fichiers et comparaison avec le shell.

## Format des résultats

### Sortie colorée
- 🟢 **VERT** : Test réussi
- 🔴 **ROUGE** : Test échoué
- 🟡 **JAUNE** : Information/Section
- 🔵 **BLEU** : Test en cours

### Vérifications automatiques
Chaque test affiche maintenant :
```
✓ File descriptors properly managed
✓ No memory leaks detected
```

### Rapport final
```
=== TEST SUMMARY ===
Total tests: 25
✅ Passed: 23
❌ Failed: 2

Log complet sauvegardé dans: pipex_tests.log
```

## Debugging

### En cas d'échec
1. Vérifiez les messages d'erreur détaillés
2. Comparez les sorties attendues vs obtenues
3. Vérifiez les codes de retour

### Tests manuels
```bash
# Test manuel simple
echo "test" | ./pipex /dev/stdin "cat" "wc -l" output.txt
cat output.txt

# Équivalent shell pour comparaison
echo "test" | cat | wc -l
```

## Makefile

Assurez-vous que votre Makefile supporte :
```makefile
# Compilation de base
make

# Nettoyage
make clean

# Compilation avec bonus (optionnel)
make bonus
```

## Fichiers de test

Les tests créent temporairement :
- `test_*.txt` : Fichiers d'entrée
- `out*_*.txt` : Fichiers de sortie
- `*_pipex.txt` / `*_shell.txt` : Comparaisons
- `pipex_tests.log` : Log centralisé unique

**Tous les fichiers temporaires sont automatiquement supprimés.**

## Amélirations récentes

### 🆕 Vérifications automatiques
- **Descripteurs de fichiers** : Vérification après chaque test
- **Fuites mémoire** : Détection avec valgrind (si disponible)
- **Performance** : Tests de stress avec monitoring

### 🆕 Logging centralisé
- Un seul fichier de log : `pipex_tests.log`
- Historique complet de tous les tests
- Informations de diagnostic détaillées

## Exemples de tests

### Test basique réussi
```
✓ PASS: Basic ls | wc -l - Output and exit code
   ✓ File descriptors properly managed
✓ PASS: grep | wc -w - Output and exit code
   ✓ File descriptors properly managed
```

### Test d'erreur réussi
```
✓ PASS: No arguments - Expected: Non-zero exit, Actual: Exit code: 1
   ✓ File descriptors properly managed
✓ PASS: Invalid command - Expected: Non-zero exit, Actual: Exit code: 127
   ✓ File descriptors properly managed
```

### Test échoué
```
✗ FAIL: Complex pipe - Output
  Expected: Files match
  Actual: Files differ
  Pipex output: (content shown)
  Shell output: (content shown)
   ✗ Possible FD leak detected (before: 5, after: 7)
```

### Fuite détectée
```
✗ FAIL: Memory test - FD check
  Expected: No FD leaks
  Actual: Possible leak
   ✗ Possible FD leak in: Test name (before: 5, after: 7)
```

## Notes importantes

1. **Heredoc** : Les tests de heredoc nécessitent que votre implementation supporte le comportement d'append (`>>`)

2. **Multiple pipes** : Pour les bonus, votre pipex doit supporter plus de 2 commandes

3. **Codes d'erreur** : Respectez les codes de retour Unix standards :
   - `0` : Succès
   - `1` : Erreur générale
   - `127` : Commande non trouvée

4. **Performance** : Les tests de performance sont informatifs, pas obligatoires

5. **Vérifications automatiques** : Chaque test vérifie maintenant :
   - Gestion des descripteurs de fichiers (automatique)
   - Fuites mémoire avec valgrind (si disponible)
   - Codes de retour appropriés

## Troubleshooting

### Compilation échoue
```bash
cd ..
make clean
make
```

### Permission denied
```bash
chmod +x test/*.sh
```

### Tests bloqués
Utilisez `Ctrl+C` pour interrompre, les fichiers temporaires seront nettoyés.

### Diagnostic des fuites
Si des fuites de descripteurs sont détectées :
```bash
# Vérifiez manuellement
lsof -p $$
./pipex infile "cmd1" "cmd2" outfile
lsof -p $$
```

### Performance dégradée
Les vérifications automatiques ajoutent ~0.1s par test. Pour des tests rapides :
```bash
# Éditez temporairement verify_memory_and_fd dans les scripts
# et commentez les vérifications lsof
```

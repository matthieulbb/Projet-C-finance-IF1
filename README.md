# C++ for Finance — Projet 2026 (ESILV)

## Structure du dépôt

```
cpp-finance-project/
├── src/                        code du projet (rendu)
├── tests/                      fichiers de test (non rendus)
├── scripts/
│   └── make_submission.sh      génère le zip de rendu
├── CMakeLists.txt              instructions de compilation
├── .github/workflows/build.yml compilation automatique sur GitHub
├── .gitignore                  fichiers ignorés par Git
└── README.md                   ce fichier
```

> Sur Mac, les fichiers commençant par un point sont cachés. Dans le Finder : **Cmd + Maj + .** pour les afficher.

### `src/` — le code du projet

Tous les fichiers `.h` et `.cpp` du projet (`Option.h`, `BlackScholesPricer.cpp`, …).
C'est **le seul dossier envoyé au professeur**. Il ne doit contenir **aucun `main()`**, puisque la correction utilise le sien.

Le fichier `.gitkeep` est vide : il sert seulement à ce que Git conserve le dossier tant qu'il n'y a pas de code.

### `tests/` — nos tests

Nos propres programmes de test, chacun avec son `main()`, ainsi que les `main()` de correction fournis sur DVL.
Ce dossier **n'est jamais rendu**. Séparer `src/` et `tests/` permet de tester le code sans risquer de laisser un `main()` dans le rendu.

`main_sanity.cpp` affiche simplement « Build OK » : il vérifie que la compilation fonctionne.

### `scripts/make_submission.sh` — le rendu

```bash
./scripts/make_submission.sh NOM_DU_REPRESENTANT
```

Il prend uniquement les `.h`/`.cpp` de `src/`, refuse de continuer s'il trouve un `main()`, crée `NOM_DU_REPRESENTANT.zip` et vérifie que le zip fait moins de 1 Mo.

### `CMakeLists.txt` — la compilation

Indique à CMake comment compiler le projet sur n'importe quelle machine (Mac, Windows, Linux) :
- C++17, avec tous les avertissements du compilateur activés ;
- chaque fichier de `tests/` devient un programme qui combine ce test avec tout le code de `src/`.

Ajouter `tests/test_bs.cpp` crée donc automatiquement le programme `build/test_bs`, sans rien modifier.

```bash
cmake -S . -B build        # une seule fois
cmake --build build        # à chaque modification
./build/main_sanity
```

### `.github/workflows/build.yml` — vérification automatique

À chaque push, GitHub compile le projet sur Linux, Mac et Windows. Une croix rouge dans l'onglet **Actions** signale un code qui ne compile pas, y compris sur une machine différente de la sienne.

### `.gitignore` — fichiers ignorés

Fichiers que Git ne suit pas, car propres à chaque machine ou régénérables : dossier `build/`, zip de rendu, réglages d'éditeur (`.vscode/`, `.idea/`), `.DS_Store` de macOS.

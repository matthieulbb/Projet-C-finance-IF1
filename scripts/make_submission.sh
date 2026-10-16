#!/usr/bin/env bash
# Prépare le zip de rendu selon les consignes du sujet :
#  - uniquement les .cpp et .h de src/
#  - aucune fonction main()
#  - taille < 1 Mo
#  - nommé d'après le nom de famille du représentant de l'équipe
#
# Usage : ./scripts/make_submission.sh NOM_DU_REPRESENTANT

set -euo pipefail

if [ $# -ne 1 ]; then
  echo "Usage : $0 NOM_DU_REPRESENTANT" >&2
  exit 1
fi

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
SRC="$ROOT/src"
OUT="$ROOT/$1.zip"

shopt -s nullglob
FILES=("$SRC"/*.cpp "$SRC"/*.h)
if [ ${#FILES[@]} -eq 0 ]; then
  echo "Erreur : aucun fichier .cpp/.h dans src/" >&2
  exit 1
fi

# Fichiers non autorisés dans src/
OTHER=$(find "$SRC" -type f ! -name '*.cpp' ! -name '*.h' ! -name '.gitkeep')
if [ -n "$OTHER" ]; then
  echo "Attention, fichiers ignorés (ni .cpp ni .h) :" >&2
  echo "$OTHER" >&2
fi

# Aucun main() autorisé
if grep -nE '\bint[[:space:]]+main[[:space:]]*\(' "${FILES[@]}"; then
  echo "Erreur : une fonction main() est présente dans src/ (voir ci-dessus)." >&2
  exit 1
fi

rm -f "$OUT"
(cd "$SRC" && zip -q "$OUT" $(cd "$SRC" && ls *.cpp *.h 2>/dev/null))

SIZE=$(wc -c < "$OUT")
if [ "$SIZE" -gt 1048576 ]; then
  echo "Erreur : le zip fait $SIZE octets (> 1 Mo)." >&2
  exit 1
fi

echo "OK : $OUT ($SIZE octets)"
echo "Rappel : objet du mail = \"C++ for Finance project 2026\", envoi au prof de TD, toute l'équipe en CC."

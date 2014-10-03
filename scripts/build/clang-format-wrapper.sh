#!/usr/bin/env sh

clang_format_bin=clang-format
if command -v clang-format-3.5 >/dev/null; then
    clang_format_bin=clang-format-3.5
fi

files="$@"

for file in $files; do
    $clang_format_bin -style=file "$file" | \
        sed -E 's/^( *)} (else|catch)(()|.*[^\\])$/\1}\
\1\2\3/g' > "$file.clangformat"
    git diff --no-index "$file" "$file.clangformat" | cat
    rm "$file.clangformat"
done

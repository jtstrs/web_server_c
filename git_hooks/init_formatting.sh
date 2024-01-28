hooks_patch="../.git/hooks/pre_commit"
touch $hooks_patch

formatting_sequence="find . -regex '.*\.\(c\|h\)' -exec clang-format -style=file -i {} \;"

echo $formatting_sequence >> "$hooks_patch"
# Makefile.am patching
 * Fix `Makefile.am` with modifications you need, save as `Makefile_teserakt.am`
 * Generate patch ```diff -u Makefile.am Makefile_fitcrack.am > Makefile.am.patch```
 * Apply if need be ```patch -p0 Makefile.am Makefile.am.patch```

#!/bin/bash

cat header > shanty.html
markdown_py shanty.md >> shanty.html
echo "</body></html>" >> shanty.html

sed -e 's/\.\/img/\.\/doc\/img/g' shanty.md > ../README.md

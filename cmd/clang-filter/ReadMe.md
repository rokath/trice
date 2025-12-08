# Clang Files

* The Trice folder contains also some vendor files, especially in the examples.
* When calling clang-format.sh we do not want modify those files.
* Therefore there is a file .clang-format-ignore and this code here is used inside clang-format.sh to parse clang-format.sh to exclude what should not be touched.


# Steps to re-generate these testfiles
- open terminal here
- generate til.json `trice u -src ../../../srcTrice.C/triceCheck.c`
- generate position.out `trice check -list .\til.json -color off > position.out` 
- generate negative.out `trice check -list .\til.json -color off -dataset negative > negative.out` 
- make sure *.out are in UTF-8 format for successful tests

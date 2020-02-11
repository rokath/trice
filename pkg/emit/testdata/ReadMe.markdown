
# Steps to re-generate these testfiles
- open terminal here
- generate til.json `trice u -src ../../../testdata/traceLogDemoF030R8/Src/traceLogCheck.c`
- tilDez.json and tilHex.json are manually derived from til.json
- generate NegativeDez.out `trice check -list .\triceiDez.json -color off -dataset negative > NegativeDez.out` (as example)
- make sure *.out are in UTF-8 format for successful tests

// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// not needed
//func runEsc2(sw *emitter.TriceLineComposer, list *id.List) {
//again: // (re-)setup input port
//	portReader, e := decoder.NewReader(Port, "")
//	if nil != e {
//		if verbose {
//			fmt.Println(e)
//		}
//		time.Sleep(1000 * time.Millisecond)
//		goto again
//	}
//	defer portReader.Close()
//	if showInputBytes {
//		portReader = newBytesViewer(portReader)
//	}
//	//var dec decoder.StringsReader = decoder.NewEsc(list, portReader)
//}
//
//
// This works well but is not well refactored
//func receiveEscTricesAndDisplayAnsiColor(
//	sw *emitter.TriceLineComposer,
//	rd io.ReadCloser,
//	list *id.List,
//	hardReadError chan bool) (et *translator.EscTranslator) {
//
//	// uses rd for reception and the io.StringWriter interface sw for writing.
//	// collects trice bytes to a complete esc trice message, generates the appropriate string using list and writes it to the provided io.StringWriter
//	et = translator.NewEscTrices(sw, list, rd, hardReadError)
//	return
//}
//

/*
func receiveBareSimpleTricesAndDisplayAnsiColor(
	sw *emitter.TriceLineComposer,
	rd io.ReadCloser,
	list *id.List,
	hardReadError chan bool) (bt *translator.BareTranslator) {

	// triceAtomsReceiver uses the io.Reader interface from s and implements the TriceAtomsReceiver interface.
	// It scans the raw input byte stream and decodes the trice atoms it transmits to the TriceAtomsReceiver interface.
	triceAtomsReceiver := receiver.NewTricesfromBare(rd, hardReadError)

	// bt uses triceAtomsReceiver for reception and the io.StringWriter interface sw for writing.
	// It collects trice atoms to a complete trice, generates the appropriate string using list and writes it to the provided io.StringWriter
	bt = translator.NewSimpleTrices(sw, list, triceAtomsReceiver)
	return
}
*/

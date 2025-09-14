package cgot

// For some reason inside the trice_test.go an 'import "C"' is not possible.

// char* TargetActivity( void );
import "C"

func targetActivity() (r string) {
	return C.GoString(C.TargetActivity())
}

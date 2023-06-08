// generated code - do not edit!

// There is still a need to exchange the format specifier from C to C#.
// See https://stackoverflow.com/questions/33432341/how-to-use-c-language-format-specifiers-in-c-sharp
// and https://www.codeproject.com/Articles/19274/A-printf-implementation-in-C for possible help.

namespace TriceIDList;

    public class TilItem
    {
        public TilItem(string strg, int bitWidth, int size) 
        {
            Strg = strg;
            BitWidth = bitWidth;
            Size = size;
        } 

        public string Strg { get; init; }
        public int BitWidth { get; init; }
        public int Size { get; init; }
    }
    
    public static class Til 
    {
        public static readonly Dictionary<int, TilItem> TilList= new Dictionary<int, TilItem>
        { // id, TilItem ( Strg, bitWidth, dataLength )
    };
}


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
                                                                //id, TilItem
    public static readonly Dictionary<int, TilItem> TilList= new Dictionary<int, TilItem>
    {
        { 1, new TilItem("a", 8, 16) },
        { 2, new TilItem("b", 8, 16) },
        { 3, new TilItem("New Value {0:d}, {1:d}, {2:d}, {3:d}", 8, 16) },
        { 4, new TilItem("New Value {0:d}", 8, 16) },
        { 5, new TilItem("e", 8, 16) }
    };
}


using System.Runtime.InteropServices;
namespace GoonEngine.Models;

[StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
public struct Overlap
{
    public int OverlapDirection;
    public IntPtr OverlapBody;
}


public enum OverlapDirections
{
    gpOverlapNoOverlap = 0,
    gpOverlapUp,
    gpOverlapRight,
    gpOverlapDown,
    gpOverlapLeft

}
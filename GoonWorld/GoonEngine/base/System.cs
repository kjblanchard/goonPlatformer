using System.Net;
using System.Runtime.InteropServices;
namespace GoonEngine;

public abstract class System
{
    public System(int type)
    {
        _type = type;
    }
    int _type;
    public virtual void Start() { }
    public void Update(ref IntPtr context, int type, IntPtr data)
    {
        ComponentForEach();
    }
    protected virtual void ComponentUpdate(IntPtr component) { }
    public void RegisterInECS()
    {
        ECS.System.NewSystem(Update, _type);
    }
    public unsafe void ComponentForEach()
    {
        Console.WriteLine("Hello bro");
        var componentsPtr = ECS.GetComponentsOfType(_type);
        var componentCount = ECS.GetNumComponentsOfType(_type);
        if (componentsPtr == IntPtr.Zero)
            return;
        for (int i = 0; i < componentCount; i++)
        {
            ComponentUpdate(Marshal.ReadIntPtr(componentsPtr, i * IntPtr.Size));
        }
    }

}
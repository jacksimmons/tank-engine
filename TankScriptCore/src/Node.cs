using System;
using System.Runtime.CompilerServices;


namespace Tank
{
    public class Node
    {
        public string Name
        {
            get
            {
                return "";
            }
        }

        private string m_name;
        private bool m_enabled;
        private bool m_started;


        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void GetName(IntPtr thisPtr, out string name);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void SetName(IntPtr thisPtr, string name);
    }
}

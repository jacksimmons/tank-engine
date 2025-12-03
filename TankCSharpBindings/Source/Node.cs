using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;


namespace Tank
{
    public class Node
    {
        // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
        //           CS WRAPPER
        // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

        /// <summary>
        /// Pointer to the native Node object.
        /// </summary>
        private IntPtr m_ptr;


        public string Name
        {
            get
            {
                GetName(m_ptr, out string name);
                return name;
            }
            set
            {
                SetName(m_ptr, value);
            }
        }


        // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
        //          CPP METHODS
        // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

        [DllImport("TankEngine.dll")]
        private extern static void GetName(IntPtr thisPtr, out string name);

        [DllImport("TankEngine.dll")]
        private extern static void SetName(IntPtr thisPtr, string name);
    }
}

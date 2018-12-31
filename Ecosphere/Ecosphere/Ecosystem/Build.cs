using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace bio.eco
{
    public enum BuildMode
    {
        NSS,
        NRO,
        NSO,
        KIP,
        Library,
        Application,
    }

    public abstract class Build
    {
        public BuildMode Mode { get; set; }

        public Build(BuildMode Mode)
        {
            this.Mode = Mode;
        }
    }

    public class NSSBuild : Build
    {
        public NSSBuild() : base(BuildMode.NSS)
        {
        }
    }

    public class NSOBuild : Build
    {
        public string JSON { get; set; }
        public string NPDM { get; set; }

        public NSOBuild() : base(BuildMode.NSO)
        {
        }
    }

    public class NROBuild : Build
    {
        public string ApplicationId { get; set; }
        public string Name { get; set; }
        public string Author { get; set; }
        public string Version { get; set; }
        public string Icon { get; set; }
        public string NACP { get; set; }
        public string RomFs { get; set; }

        public NROBuild() : base(BuildMode.NRO)
        {
        }
    }

    public class KIPBuild : Build
    {
        public string JSON { get; set; }

        public KIPBuild() : base(BuildMode.KIP)
        {
        }
    }

    public class LibraryBuild : Build
    {
        public string Target { get; set; }

        public LibraryBuild() : base(BuildMode.Library)
        {
        }
    }

    public class ApplicationBuild : Build
    {
        public string ApplicationId { get; set; }
        public string Name { get; set; }
        public string Author { get; set; }
        public string Version { get; set; }
        public string Icon { get; set; }
        public string JSON { get; set; }
        public string NACP { get; set; }
        public string RomFs { get; set; }
        public string KeyGeneration { get; set; }
        public string KeySet { get; set; }

        public ApplicationBuild() : base(BuildMode.Application)
        {
        }
    }
}

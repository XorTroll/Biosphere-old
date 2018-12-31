using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace bio.eco
{
    public class BiosphereException : Exception
    {
        public BiosphereException(string Error) : base(Error)
        {
        }
    }

    public class ProjectException : Exception
    {
        public ProjectException(string Error) : base(Error)
        {
        }
    }

    public class ConfigException : Exception
    {
        public ConfigException(string Error) : base(Error)
        {
        }
    }

    public class CompileException : Exception
    {
        public CompileException(string Error) : base(Error)
        {
        }
    }
}

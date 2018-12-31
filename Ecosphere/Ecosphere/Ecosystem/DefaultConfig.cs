using System;
using System.Linq;
using System.IO;
using System.Xml.Linq;
using System.Threading.Tasks;

namespace bio.eco
{
    public class DefaultConfig
    {
        public string Architecture { get; set; }
        public string C { get; set; }
        public string Cpp { get; set; }
        public string CppStd { get; set; }

        public static DefaultConfig GetDefaultConfig()
        {
            string cfgxml = Utils.GetCwd() + "\\DefaultConfig.xml";
            if(!File.Exists(cfgxml)) throw new ProjectException("Default config XML data file does not exist.");
            DefaultConfig cfg = new DefaultConfig();
            XDocument doc = XDocument.Load(cfgxml);
            XElement root = doc.Element("DefaultConfig");
            if(root == null) throw new ProjectException("Invalid root element of the config XML file.");
            XElement arch = root.Element("Architecture");
            Utils.CheckExists(arch, "Architecture");
            cfg.Architecture = arch.Value;
            XElement c = root.Element("C");
            Utils.CheckExists(c, "C");
            cfg.C = c.Value;
            XElement cpp = root.Element("Cpp");
            Utils.CheckExists(cpp, "Cpp");
            cfg.Cpp = cpp.Value;
            XElement cppstd = root.Element("CppStd");
            Utils.CheckExists(cppstd, "CppStd");
            cfg.CppStd = cppstd.Value;
            return cfg;
        }
    }
}

using System;
using System.IO;
using System.Xml.Linq;
using System.Reflection;
using System.Diagnostics;

namespace bio.eco
{
    public static class Utils
    {
        public static void CheckExists(XElement Element, string ProperName)
        {
            if(Element == null) throw new ConfigException("Required property was not found: \'" + ProperName + "\'");
        }

        public static string GetCwd()
        {
            return Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
        }

        public static string GetdevKitProDirectory()
        {
            string cwd = GetCwd();
            if(Path.GetFileNameWithoutExtension(cwd) != "bin") throw new BiosphereException("Ecosystem compiler is not correctly located. Should be in \"[DEVKITPRO]/Biosphere/Ecosphere/bin\".");
            if(Path.GetFileName(Path.GetDirectoryName(cwd)) != "Ecosphere") throw new BiosphereException("Ecosystem compiler is not correctly located. Should be in \"[DEVKITPRO]/Biosphere/Ecosphere/bin\".");
            if(Path.GetFileName(Path.GetDirectoryName(Path.GetDirectoryName(cwd))) != "Biosphere") throw new BiosphereException("Ecosystem compiler is not correctly located. Should be in \"[DEVKITPRO]/Biosphere/Ecosphere/bin\".");
            return Path.GetFullPath(Path.Combine(cwd, "..", "..", ".."));
        }

        public static string CreateTempDirectory()
        {
            string path = Path.Combine(Path.GetTempPath(), Path.GetTempFileName());
            if(File.Exists(path)) File.Delete(path);
            Directory.CreateDirectory(path);
            return path;
        }

        public static string ExecuteCommand(string File, string Args, bool Wait)
        {
            ProcessStartInfo info = new ProcessStartInfo()
            {
                FileName = File,
                Arguments = Args,
                CreateNoWindow = true,
                UseShellExecute = false,
                RedirectStandardError = true,
            };
            Process p = Process.Start(info);
            if(Wait) p.WaitForExit();
            string data = null;
            if(p.ExitCode != 0) data = p.StandardError.ReadToEnd();
            return data;
        }
    }
}

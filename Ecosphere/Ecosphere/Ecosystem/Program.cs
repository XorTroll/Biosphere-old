using System;
using System.IO;
using System.Reflection;
using System.Threading;

namespace bio.eco
{
    public class Program
    {
        public static int Main(string[] Args)
        {
            string proj = "";
            if(Args == null || Args.Length == 0) proj = "D:\\Homebrew\\Projects\\Biosphere\\Biosphere";
            else proj = Args[0];
            try
            {
                Console.WriteLine();
                Console.WriteLine(" ** Ecosystem - Ecosphere compiler, part of Biosphere project **");
                Console.WriteLine();
                Console.WriteLine("[eco:Information] Processing directory: \"" + proj + "\"");
                Project prj = new Project(proj);
                Console.WriteLine();
                Console.WriteLine("[eco:Information] Project description:");
                Console.WriteLine();
                Console.WriteLine(" -- Project name: " + prj.ProjectName);
                Console.WriteLine(" -- Output folder: " + Path.Combine(prj.Directory, prj.Output));
                // Escribir mas cosas
                Console.WriteLine();
                Console.WriteLine("[eco:Information] Building project...");
                prj.Compile();
                Console.WriteLine("[eco:Information] Project was successfully built.");
            }
            catch(BiosphereException bioex)
            {
                Console.WriteLine();
                Console.WriteLine("[eco:BiosphereError] Caught Biosphere error:");
                Console.WriteLine(bioex.Message);
                Thread.Sleep(2000);
            }
            catch(ProjectException prjex)
            {
                Console.WriteLine();
                Console.WriteLine("[eco:ProjectError] Caught project error:");
                Console.WriteLine(prjex.Message);
                Thread.Sleep(2000);
            }
            catch(ConfigException cfgex)
            {
                Console.WriteLine();
                Console.WriteLine("[eco:ConfigError] Caught project config error:");
                Console.WriteLine(cfgex.Message);
                Thread.Sleep(2000);
            }
            catch(CompileException cmpex)
            {
                Console.WriteLine();
                Console.WriteLine("[eco:CompileError] Caught compile time error:");
                Console.WriteLine(cmpex.Message);
                Thread.Sleep(2000);
            }
            catch(Exception ex)
            {
                Console.WriteLine();
                Console.WriteLine("[eco:Error] Caught generic error (" + ex.GetType().ToString() + "):");
                Console.WriteLine(ex.Message);
                Thread.Sleep(2000);
            }
            Console.ReadKey();
            return 0;
        }
    }
}

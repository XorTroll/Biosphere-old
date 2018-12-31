using System;
using System.Collections.Generic;
using System.Linq;
using System.Xml.Linq;
using System.IO;
using Microsoft.VisualBasic.FileIO;

namespace bio.eco
{
    public class Project
    {
        public string Directory { get; set; }
        public string ProjectName { get; set; }
        public string Output { get; set; }
        public bool IncludeBio { get; set; }
        public List<string> Includes { get; set; }
        public List<string> Sources { get; set; }
        public List<Build> Builds { get; set; }

        public Project(string Path)
        {
            if(System.IO.Path.GetDirectoryName(Path) != null) Path = Path.TrimEnd(System.IO.Path.DirectorySeparatorChar, System.IO.Path.AltDirectorySeparatorChar);
            Directory = Path;
            ProjectName = System.IO.Path.GetFileName(Path);
            string[] files = System.IO.Directory.GetFiles(Path);
            if(files == null || files.Length == 0) throw new ProjectException("No XML project file was found.");
            string configxml = "";
            foreach(string file in files)
            {
                if(System.IO.Path.GetExtension(file).Replace(".", "").ToLower() == "xml")
                {
                    configxml = file;
                    break;
                }
            }
            if(string.IsNullOrEmpty(configxml)) throw new ProjectException("No XML project file was found.");
            XDocument xml = XDocument.Load(configxml);
            XElement root = xml.Element("BiosphereProject");
            if(root == null) throw new ConfigException("Invalid root element of the project XML file.");
            IncludeBio = true;
            XElement hbio = root.Element("IncludeBio");
            bool incbio = false;
            if(hbio != null)
            {
                if(bool.TryParse(hbio.Value, out incbio)) IncludeBio = incbio;
            }
            XElement output = root.Element("Output");
            Utils.CheckExists(output, "Output");
            Output = output.Value;
            XElement includes = root.Element("Includes");
            Utils.CheckExists(includes, "Includes");
            Includes = new List<string>();
            var allincludes = includes.Elements("Include");
            if(allincludes.Any()) foreach(var include in allincludes) Includes.Add(include.Value);
            XElement sources = root.Element("Sources");
            Sources = new List<string>();
            var allsources = sources.Elements("Source");
            if(!allsources.Any()) throw new ConfigException("No Source element was found. At least one source path is required.");
            foreach(var source in allsources) Sources.Add(source.Value);
            XElement builds = root.Element("Builds");
            Builds = new List<Build>();
            Utils.CheckExists(builds, "Builds");
            var allbuilds = builds.Elements("Build");
            if(!allbuilds.Any()) Builds.Add(new NSSBuild());
            else foreach(var build in allbuilds)
            {
                XElement bmode = build.Element("Mode");
                Utils.CheckExists(bmode, "Build.Mode");
                BuildMode m = BuildMode.NRO;
                if(!Enum.TryParse(bmode.Value, out m)) throw new ConfigException("Unknown build mode. It has to be NSO, NRO, KIP, Library or Application.");
                switch(m)
                {
                    case BuildMode.NSO:
                        XElement json = build.Element("JSON");
                        XElement npdm = build.Element("NPDM");
                        NSOBuild nsob = new NSOBuild();
                        if(json == null) nsob.JSON = null;
                        else
                        {
                            if(!File.Exists(json.Value))
                            {
                                if(File.Exists(System.IO.Path.Combine(Directory, json.Value))) nsob.JSON = System.IO.Path.Combine(Directory, json.Value);
                                else throw new CompileException("Specified JSON NPDM metadata does not exist.");
                            }
                            nsob.JSON = json.Value;
                        }
                        if(npdm == null) nsob.NPDM = null;
                        else
                        {
                            if(!File.Exists(npdm.Value))
                            {
                                if(File.Exists(System.IO.Path.Combine(Directory, npdm.Value))) nsob.NPDM = System.IO.Path.Combine(Directory, npdm.Value);
                                else throw new CompileException("Specified NPDM file does not exist.");
                            }
                            nsob.NPDM = npdm.Value;
                        }
                        if(nsob.JSON != null && nsob.NPDM != null) throw new CompileException("Both JSON NPDM metadata and NPDM file were specified. Only specifiy a JSON NPDM metadata when there is no custom NPDM file specified.");
                        if(nsob.JSON == null && nsob.NPDM == null) throw new CompileException("No NPDM file or JSON NPDM metadata were specified. This build mode requires one of these.");
                        Builds.Add(nsob);
                        break;
                    case BuildMode.NRO:
                        XElement appid = build.Element("ApplicationId");
                        Utils.CheckExists(appid, "Build.ApplicationId");
                        XElement name = build.Element("Name");
                        XElement author = build.Element("Author");
                        XElement version = build.Element("Version");
                        XElement icon = build.Element("Icon");
                        XElement nacp = build.Element("NACP");
                        XElement romfs = build.Element("RomFs");
                        NROBuild nrob = new NROBuild();
                        if(name == null) nrob.Name = "Biosphere application";
                        else nrob.Name = name.Value;
                        if(author == null) nrob.Author = "Unknown author";
                        else nrob.Author = author.Value;
                        if(version == null) nrob.Version = "Unknown version";
                        else nrob.Version = version.Value;
                        if(icon == null) nrob.Icon = Utils.GetdevKitProDirectory() + "\\Biosphere\\Ecosphere\\Application.jpg";
                        else
                        {
                            if(!File.Exists(icon.Value))
                            {
                                if(File.Exists(System.IO.Path.Combine(Directory, icon.Value))) nrob.Icon = System.IO.Path.Combine(Directory, icon.Value);
                                else throw new CompileException("Specified icon does not exist.");
                            }
                            nrob.Icon = icon.Value;
                        }
                        if(nacp == null) nrob.NACP = null;
                        else
                        {
                            if(!File.Exists(nacp.Value))
                            {
                                if(File.Exists(System.IO.Path.Combine(Directory, nacp.Value))) nrob.NACP = System.IO.Path.Combine(Directory, nacp.Value);
                                else throw new CompileException("Specified NACP file does not exist.");
                            }
                            nrob.NACP = nacp.Value;
                        }
                        if(romfs == null) nrob.RomFs = null;
                        else
                        {
                            if(!File.Exists(romfs.Value))
                            {
                                if(File.Exists(System.IO.Path.Combine(Directory, romfs.Value))) nrob.RomFs = System.IO.Path.Combine(Directory, romfs.Value);
                                else throw new CompileException("Specified RomFs directory does not exist.");
                            }
                            nrob.RomFs = romfs.Value;
                        }
                        nrob.ApplicationId = appid.Value;
                        Builds.Add(nrob);
                        break;
                    case BuildMode.KIP:
                        XElement kjson = build.Element("JSON");
                        KIPBuild kipb = new KIPBuild();
                        if(kipb == null) Utils.CheckExists(kjson, "Build.JSON");
                        else
                        {
                            if(!File.Exists(kjson.Value))
                            {
                                if(File.Exists(System.IO.Path.Combine(Directory, kjson.Value))) kipb.JSON = System.IO.Path.Combine(Directory, kjson.Value);
                                else throw new CompileException("Specified JSON KIP metadata does not exist.");
                            }
                            kipb.JSON = kjson.Value;
                        }
                        Builds.Add(kipb);
                        break;
                    case BuildMode.Library:
                        XElement tgt = build.Element("Target");
                        Utils.CheckExists(tgt, "Build.Target");
                        LibraryBuild libb = new LibraryBuild()
                        {
                            Target = tgt.Value,
                        };
                        Builds.Add(libb);
                        break;
                    case BuildMode.Application:
                        XElement aappid = build.Element("ApplicationId");
                        Utils.CheckExists(aappid, "Build.ApplicationId");
                        XElement aname = build.Element("Name");
                        XElement aauthor = build.Element("Author");
                        XElement aversion = build.Element("Version");
                        XElement aicon = build.Element("Icon");
                        XElement anacp = build.Element("NACP");
                        XElement aromfs = build.Element("RomFs");
                        XElement ajson = build.Element("JSON");
                        XElement kset = build.Element("KeySet");
                        XElement kgen = build.Element("KeyGeneration");
                        ApplicationBuild appb = new ApplicationBuild();
                        if(aname == null) appb.Name = "Biosphere application";
                        else appb.Name = aname.Value;
                        if(aauthor == null) appb.Author = "Unknown author";
                        else appb.Author = aauthor.Value;
                        if(aversion == null) appb.Version = "Unknown version";
                        else appb.Version = aversion.Value;
                        if(aicon == null) appb.Icon = Utils.GetdevKitProDirectory() + "\\Biosphere\\Ecosphere\\Application.jpg";
                        else
                        {
                            if(!File.Exists(aicon.Value))
                            {
                                if (File.Exists(System.IO.Path.Combine(Directory, aicon.Value))) appb.Icon = System.IO.Path.Combine(Directory, aicon.Value);
                                else throw new CompileException("Specified icon does not exist.");
                            }
                            appb.Icon = aicon.Value;
                        }
                        if(anacp == null) appb.NACP = null;
                        else
                        {
                            if (!File.Exists(anacp.Value))
                            {
                                if (File.Exists(System.IO.Path.Combine(Directory, anacp.Value))) appb.NACP = System.IO.Path.Combine(Directory, anacp.Value);
                                else throw new CompileException("Specified NACP file does not exist.");
                            }
                            appb.NACP = anacp.Value;
                        }
                        if(aromfs == null) appb.RomFs = null;
                        else
                        {
                            if (!File.Exists(aromfs.Value))
                            {
                                if (File.Exists(System.IO.Path.Combine(Directory, aromfs.Value))) appb.RomFs = System.IO.Path.Combine(Directory, aromfs.Value);
                                else throw new CompileException("Specified RomFs directory does not exist.");
                            }
                            appb.RomFs = aromfs.Value;
                        }
                        if(ajson == null) appb.JSON = null;
                        else
                        {
                            if(!File.Exists(ajson.Value))
                            {
                                if(File.Exists(System.IO.Path.Combine(Directory, ajson.Value))) appb.JSON = System.IO.Path.Combine(Directory, ajson.Value);
                                else throw new CompileException("Specified JSON NPDM metadata does not exist.");
                            }
                            appb.JSON = ajson.Value;
                        }

                        if(kset == null) Utils.CheckExists(kset, "Build.KeySet");
                        else
                        {
                            if(!File.Exists(kset.Value))
                            {
                                if(File.Exists(System.IO.Path.Combine(Directory, kset.Value))) appb.KeySet = System.IO.Path.Combine(Directory, kset.Value);
                                else throw new CompileException("Specified JSON NPDM metadata does not exist.");
                            }
                            appb.KeySet = kset.Value;
                        }
                        appb.ApplicationId = aappid.Value;
                        if(kgen == null) appb.KeyGeneration = "1";
                        else appb.KeyGeneration = kgen.Value;
                        appb.ApplicationId = aappid.Value;
                        Builds.Add(appb);
                        break;
                }
            }
        }

        public void Compile()
        {
            string dkp = Utils.GetdevKitProDirectory();
            if(!System.IO.Directory.Exists(dkp)) throw new CompileException("devkitPro folder does not exist.");
            if(!System.IO.Directory.Exists(dkp + "\\devkita64")) throw new CompileException("devkitA64 folder does not exist.");
            DefaultConfig dc = DefaultConfig.GetDefaultConfig();
            string builddir = Utils.CreateTempDirectory();
            string includes = "";
            if(IncludeBio) includes += "-I\"" + dkp + "\\Biosphere\\Biosphere\\include\"";
            if(Includes.Count > 0) foreach(string inc in Includes) includes += " -I\"" + Directory + "\\" + inc + "\"";
            if(Sources.Count == 0) throw new CompileException("There are no source directories to compile.");
            List<string> srcos = new List<string>();
            List<string> srcds = new List<string>();
            bool hascpp = false;
            string outcmd = null;
            foreach(string source in Sources)
            {
                string[] srcs = System.IO.Directory.GetFiles(Path.Combine(Directory, source));
                if(srcs == null || srcs.Length == 0) continue;
                System.IO.Directory.CreateDirectory(Path.Combine(builddir, source));
                foreach(string src in srcs)
                {
                    string ext = Path.GetExtension(src).Replace(".", "").ToLower();
                    string fsrcpath = Path.Combine(builddir, source, Path.GetFileName(src));
                    string srcorig = src;
                    string srco = Path.ChangeExtension(fsrcpath, "o");
                    string srcd = Path.ChangeExtension(fsrcpath, "d");
                    srcos.Add(srco);
                    srcds.Add(srcd);
                    string cmd = "";
                    if(ext == "c")
                    {
                        cmd = "-MMD -MP -MF \"" + srcd + "\" " + dc.C + " " + dc.Architecture + " " + includes + " -c \"" + srcorig + "\" -o \"" + srco + "\"";
                        outcmd = Utils.ExecuteCommand(dkp + "\\devkitA64\\bin\\aarch64-none-elf-gcc", cmd, true);
                    }
                    else if(ext == "cpp" || ext == "cxx" || ext == "cc" || ext == "c++")
                    {
                        hascpp = true;
                        cmd = "-MMD -MP -MF \"" + srcd + "\" " + dc.C + " " + dc.Architecture + " " + dc.Cpp + " -std=" + dc.CppStd + " " + includes + " -c \"" + srcorig + "\" -o \"" + srco + "\"";
                        outcmd = Utils.ExecuteCommand(dkp + "\\devkitA64\\bin\\aarch64-none-elf-g++", cmd, true);
                    }
                    else if(ext == "s" || ext == "asm")
                    {
                        cmd = "-MMD -MP -MF \"" + srcd + "\" -x assembler-with-cpp -g " + dc.Architecture + " -c \"" + srcorig + "\" -o \"" + srco + "\"";
                        outcmd = Utils.ExecuteCommand(dkp + "\\devkitA64\\bin\\aarch64-none-elf-gcc", cmd, true);
                    }
                    if(!string.IsNullOrEmpty(outcmd)) throw new CompileException("Compile errors ocurred: " + outcmd);
                }
            }
            string ofs = "";
            foreach(string osrc in srcos) ofs += "\"" + osrc + "\" ";
            string dfs = "";
            foreach (string dsrc in srcds) dfs += dsrc + " ";
            string outdir = Path.Combine(Directory, Output);
            string outtgt = Path.Combine(outdir, ProjectName);
            if(System.IO.Directory.Exists(outdir)) System.IO.Directory.Delete(outdir, true);
            System.IO.Directory.CreateDirectory(outdir);
            string linkcmd = "-specs=\"" + dkp + "\\Biosphere\\Ecosphere\\Linker.specs\" -g " + dc.Architecture + " -Wl,-Map," + Path.GetFileName(Directory) + ".map";
            linkcmd += " " + ofs;
            string outnss = outtgt + ".nss";
            if(File.Exists(outnss)) File.Delete(outnss);
            if(IncludeBio) linkcmd += " -L\"" + dkp + "\\Biosphere\\Biosphere\\lib\"";
            linkcmd += " -T \"" + dkp + "\\Biosphere\\Ecosphere\\Linker.ld\"";
            if(IncludeBio) linkcmd += " -lbio";
            linkcmd += " -o \"" + outnss + "\"";
            string prefix = "gcc";
            if(hascpp) prefix = "g++";
            string boutdir = "";
            string bouttgt = "";
            foreach(Build b in Builds)
            {
                switch(b.Mode)
                {
                    case BuildMode.NSS:
                        outcmd = Utils.ExecuteCommand(dkp + "\\devkitA64\\bin\\aarch64-none-elf-" + prefix, linkcmd, true);
                        if(!string.IsNullOrEmpty(outcmd)) throw new CompileException("Link errors ocurred: " + outcmd);
                        break;
                    case BuildMode.NSO:
                        boutdir = Path.Combine(outdir, "NSO");
                        bouttgt = Path.Combine(boutdir, ProjectName);
                        if(System.IO.Directory.Exists(boutdir)) System.IO.Directory.Delete(boutdir, true);
                        System.IO.Directory.CreateDirectory(boutdir);
                        NSOBuild nsob = b as NSOBuild;
                        outcmd = Utils.ExecuteCommand(dkp + "\\devkitA64\\bin\\aarch64-none-elf-" + prefix, linkcmd, true);
                        if(!string.IsNullOrEmpty(outcmd)) throw new CompileException("Link errors ocurred: " + outcmd);
                        string elf2nso = dkp + "\\tools\\bin\\elf2nso.exe";
                        if(!File.Exists(elf2nso)) throw new CompileException("elf2nso executable was not found.");
                        string npdmtool = dkp + "\\tools\\bin\\npdmtool.exe";
                        if(!File.Exists(npdmtool)) throw new CompileException("npdmtool executable was not found.");
                        string outnso = Path.Combine(boutdir, "main");
                        string outnpdm = Path.Combine(boutdir, "main.npdm");
                        if (string.IsNullOrEmpty(nsob.NPDM))
                        {
                            if (File.Exists(outnpdm)) File.Delete(outnpdm);
                            string npdmcmd = "\"" + nsob.JSON + "\" \"" + outnpdm + "\"";
                            outcmd = Utils.ExecuteCommand(npdmtool, npdmcmd, true);
                            if (!string.IsNullOrEmpty(outcmd)) throw new CompileException("NPDM building errors ocurred: " + outcmd);
                        }
                        else File.Copy(nsob.NPDM, outnpdm);
                        string nsocmd = "\"" + outtgt + ".nss\" \"" + outnso + "\"";
                        outcmd = Utils.ExecuteCommand(elf2nso, nsocmd, true);
                        if(!string.IsNullOrEmpty(outcmd)) throw new CompileException("NSO building errors ocurred: " + outcmd);
                        break;
                    case BuildMode.NRO:
                        boutdir = Path.Combine(outdir, "NRO");
                        bouttgt = Path.Combine(boutdir, ProjectName);
                        if(System.IO.Directory.Exists(boutdir)) System.IO.Directory.Delete(boutdir, true);
                        System.IO.Directory.CreateDirectory(boutdir);
                        NROBuild nrob = b as NROBuild;
                        outcmd = Utils.ExecuteCommand(dkp + "\\devkitA64\\bin\\aarch64-none-elf-" + prefix, linkcmd, true);
                        if(!string.IsNullOrEmpty(outcmd)) throw new CompileException("Link errors ocurred: " + outcmd);
                        string elf2nro = dkp + "\\tools\\bin\\elf2nro.exe";
                        if(!File.Exists(elf2nro)) throw new CompileException("elf2nro executable was not found.");
                        string nacptool = dkp + "\\tools\\bin\\nacptool.exe";
                        if(!File.Exists(nacptool)) throw new CompileException("nacptool executable was not found.");
                        string outnro = boutdir + "\\" + ProjectName + ".nro";
                        if(File.Exists(outnro)) File.Delete(outnro);
                        string outnacp = boutdir + "\\" + ProjectName + ".nacp";
                        if(string.IsNullOrEmpty(nrob.NACP))
                        {
                            if(File.Exists(outnacp)) File.Delete(outnacp);
                            string nacpcmd = "--create \"" + nrob.Name + "\" \"" + nrob.Author + "\" \"" + nrob.Version + "\" \"" + outnacp + "\" --titleid=" + nrob.ApplicationId;
                            outcmd = Utils.ExecuteCommand(nacptool, nacpcmd, true);
                            if(!string.IsNullOrEmpty(outcmd)) throw new CompileException("NACP building errors ocurred: " + outcmd);
                        }
                        string nrocmd = "\"" + outtgt + ".nss\" \"" + outnro + "\" --nacp=";
                        if(string.IsNullOrEmpty(nrob.NACP)) nrocmd += "\"" + outnacp + "\"";
                        else nrocmd += "\"" + nrob.NACP + "\"";
                        nrocmd += " --icon=\"" + nrob.Icon + "\"";
                        if(!string.IsNullOrEmpty(nrob.RomFs)) nrocmd += " --romfsdir=\"" + nrob.RomFs + "\"";
                        outcmd = Utils.ExecuteCommand(elf2nro, nrocmd, true);
                        if(!string.IsNullOrEmpty(outcmd)) throw new CompileException("NRO building errors ocurred: " + outcmd);
                        break;
                    case BuildMode.KIP:
                        boutdir = Path.Combine(outdir, "KIP");
                        bouttgt = Path.Combine(boutdir, ProjectName);
                        if(System.IO.Directory.Exists(boutdir)) System.IO.Directory.Delete(boutdir, true);
                        System.IO.Directory.CreateDirectory(boutdir);
                        KIPBuild kipb = b as KIPBuild;
                        outcmd = Utils.ExecuteCommand(dkp + "\\devkitA64\\bin\\aarch64-none-elf-" + prefix, linkcmd, true);
                        if(!string.IsNullOrEmpty(outcmd)) throw new CompileException("Link errors ocurred: " + outcmd);
                        string elf2kip = dkp + "\\tools\\bin\\elf2kip.exe";
                        if(!File.Exists(elf2kip)) throw new CompileException("elf2kip executable was not found.");
                        string outkip = bouttgt + ".kip";
                        string kipcmd = "\"" + outtgt + ".nss\" \"" + kipb.JSON + "\" \"" + outkip + "\"";
                        outcmd = Utils.ExecuteCommand(elf2kip, kipcmd, true);
                        if(!string.IsNullOrEmpty(outcmd)) throw new CompileException("KIP building errors ocurred: " + outcmd);
                        break;
                    case BuildMode.Library:
                        boutdir = Path.Combine(outdir, "Library");
                        bouttgt = Path.Combine(boutdir, ProjectName);
                        if(System.IO.Directory.Exists(boutdir)) System.IO.Directory.Delete(boutdir, true);
                        System.IO.Directory.CreateDirectory(boutdir);
                        LibraryBuild libb = b as LibraryBuild;
                        if (System.IO.Directory.Exists(boutdir + "\\include")) System.IO.Directory.Delete(boutdir + "\\include", true);
                        System.IO.Directory.CreateDirectory(boutdir + "\\include");
                        if(System.IO.Directory.Exists(boutdir + "\\lib")) System.IO.Directory.Delete(boutdir + "\\lib", true);
                        System.IO.Directory.CreateDirectory(boutdir + "\\lib");
                        string arcmd = "-rc \"" + boutdir + "\\lib\\lib" + libb.Target + ".a\" " + ofs;
                        outcmd = Utils.ExecuteCommand(dkp + "\\devkitA64\\bin\\aarch64-none-elf-gcc-ar", arcmd, true);
                        if(!string.IsNullOrEmpty(outcmd)) throw new CompileException("Library archive errors ocurred: " + outcmd);
                        if(Includes.Count > 0) foreach(string inc in Includes)
                        {
                            try
                            {
                                FileSystem.CopyDirectory(Path.Combine(Directory, inc), Path.Combine(boutdir, inc));
                            }
                            catch
                            {
                                continue;
                            }
                        }
                        break;
                    case BuildMode.Application:
                        boutdir = Path.Combine(outdir, "Application");
                        bouttgt = Path.Combine(boutdir, ProjectName);
                        if(System.IO.Directory.Exists(boutdir)) System.IO.Directory.Delete(boutdir, true);
                        System.IO.Directory.CreateDirectory(boutdir);
                        ApplicationBuild appb = b as ApplicationBuild;
                        outcmd = Utils.ExecuteCommand(dkp + "\\devkitA64\\bin\\aarch64-none-elf-" + prefix, linkcmd, true);
                        if(!string.IsNullOrEmpty(outcmd)) throw new CompileException("Link errors ocurred: " + outcmd);
                        string aelf2nso = dkp + "\\tools\\bin\\elf2nso.exe";
                        if(!File.Exists(aelf2nso)) throw new CompileException("elf2nso executable was not found.");
                        string anacptool = dkp + "\\tools\\bin\\nacptool.exe";
                        if(!File.Exists(anacptool)) throw new CompileException("nacptool executable was not found.");
                        string aoutnso = Path.Combine(boutdir, "ExeFs", "main");
                        if(File.Exists(aoutnso)) File.Delete(aoutnso);
                        if(System.IO.Directory.Exists(boutdir + "\\Control")) System.IO.Directory.Delete(boutdir + "\\Control", true);
                        System.IO.Directory.CreateDirectory(boutdir + "\\Control");
                        string aoutnacp = boutdir + "\\Control\\control.nacp";
                        if(string.IsNullOrEmpty(appb.NACP))
                        {
                            if(File.Exists(aoutnacp)) File.Delete(aoutnacp);
                            string nacpcmd = "--create \"" + appb.Name + "\" \"" + appb.Author + "\" \"" + appb.Version + "\" \"" + aoutnacp + "\" --titleid=" + appb.ApplicationId;
                            outcmd = Utils.ExecuteCommand(anacptool, nacpcmd, true);
                            if (!string.IsNullOrEmpty(outcmd)) throw new CompileException("NACP building errors ocurred: " + outcmd);
                        }
                        if(System.IO.Directory.Exists(boutdir + "\\ExeFs")) System.IO.Directory.Delete(boutdir + "\\ExeFs", true);
                        System.IO.Directory.CreateDirectory(boutdir + "\\ExeFs");
                        string aoutnpdm = Path.Combine(boutdir, "ExeFs", "main.npdm");
                        string anpdmtool = dkp + "\\tools\\bin\\npdmtool.exe";
                        if(!File.Exists(anpdmtool)) throw new CompileException("npdmtool executable was not found.");
                        if(File.Exists(aoutnpdm)) File.Delete(aoutnpdm);
                        string anpdmcmd = "\"" + appb.JSON + "\" \"" + aoutnpdm + "\"";
                        outcmd = Utils.ExecuteCommand(anpdmtool, anpdmcmd, true);
                        if(!string.IsNullOrEmpty(outcmd)) throw new CompileException("NPDM building errors ocurred: " + outcmd);
                        string ansocmd = "\"" + outtgt + ".nss\" \"" + aoutnso + "\"";
                        outcmd = Utils.ExecuteCommand(aelf2nso, ansocmd, true);
                        if(!string.IsNullOrEmpty(outcmd)) throw new CompileException("NSO building errors ocurred: " + outcmd);
                        bool hasromfs = false;
                        if(appb.RomFs != null) hasromfs = true;
                        if(hasromfs) FileSystem.CopyDirectory(appb.RomFs, boutdir + "\\RomFs");
                        File.Copy(appb.Icon, boutdir + "\\Control\\icon_AmericanEnglish.dat");
                        File.Copy(appb.Icon, boutdir + "\\Control\\icon_BritishEnglish.dat");
                        File.Copy(appb.Icon, boutdir + "\\Control\\icon_Japanese.dat");
                        File.Copy(appb.Icon, boutdir + "\\Control\\icon_French.dat");
                        File.Copy(appb.Icon, boutdir + "\\Control\\icon_German.dat");
                        File.Copy(appb.Icon, boutdir + "\\Control\\icon_LatinAmericanSpanish.dat");
                        File.Copy(appb.Icon, boutdir + "\\Control\\icon_Spanish.dat");
                        File.Copy(appb.Icon, boutdir + "\\Control\\icon_Italian.dat");
                        File.Copy(appb.Icon, boutdir + "\\Control\\icon_Dutch.dat");
                        File.Copy(appb.Icon, boutdir + "\\Control\\icon_CanadianFrench.dat");
                        File.Copy(appb.Icon, boutdir + "\\Control\\icon_Portuguese.dat");
                        File.Copy(appb.Icon, boutdir + "\\Control\\icon_Russian.dat");
                        File.Copy(appb.Icon, boutdir + "\\Control\\icon_Korean.dat");
                        File.Copy(appb.Icon, boutdir + "\\Control\\icon_Taiwanese.dat");
                        File.Copy(appb.Icon, boutdir + "\\Control\\icon_Chinese.dat");
                        string hbpack = Utils.GetCwd() + "\\hacbrewpack.exe";
                        if(!File.Exists(hbpack)) throw new CompileException("hacBrewPack executable does not exist.");
                        string hbcmd = "-k \"" + appb.KeySet + "\" --exefsdir=\"" + boutdir + "\\ExeFs\"";
                        if(hasromfs) hbcmd += " --romfsdir=\"" + boutdir + "\\RomFs\"";
                        else hbcmd += " --noromfs";
                        hbcmd += " --nologo --controldir=\"" + boutdir + "\\Control\" --keygeneration=" + appb.KeyGeneration + " --nspdir=\"" + boutdir + "\"";
                        outcmd = Utils.ExecuteCommand(hbpack, hbcmd, true);
                        if(!string.IsNullOrEmpty(outcmd)) throw new CompileException("NSP building errors ocurred: " + outcmd);
                        string[] files = System.IO.Directory.GetFiles(boutdir);
                        if(files == null || files.Length == 0) throw new ProjectException("No NSP file was found.");
                        foreach(string file in files)
                        {
                            if(Path.GetExtension(file).Replace(".", "").ToLower() == "nsp")
                            {
                                File.Move(file, boutdir + "\\" + ProjectName + ".nsp");
                                break;
                            }
                        }
                        break;
                }
            }
        }
    }
}

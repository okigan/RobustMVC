//<package id = "hello_world">
//  <job id = "main">
//    <script type="text/javascript">
var wshell = CreateObject("WScript.Shell");

var username = wshell.ExpandEnvironmentStrings("%USERNAME%");
var userprofile = wshell.ExpandEnvironmentStrings("%USERPROFILE%");
var localappdata = wshell.ExpandEnvironmentStrings("%LOCALAPPDATA%");
var p = userprofile + "\\Documents\\Visual Studio 2010\\Wizards\\RobustMVCWz.vsz";

WScript.Echo(p);

var fso = new ActiveXObject("Scripting.FileSystemObject");

var s = fso.CreateTextFile(p, true);

s.WriteLine('VSWIZARD 7.0');
s.WriteLine('Wizard=VsWizard.VsWizardEngine.10.0');

s.WriteLine('Param="WIZARD_NAME = RobustMVC"');
s.WriteLine('Param="WIZARD_UI = FALSE"');
s.WriteLine('Param="FALLBACK_LCID = 1033"');
s.WriteLine('Param="ABSOLUTE_PATH = ' + localappdata  + '\\RobustMVC"');


s.Close();
//    </script>
//  </job>
//</package>
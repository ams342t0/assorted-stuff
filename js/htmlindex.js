var fso;
var tf;
var f;
var wsh = WScript.CreateObject("Wscript.Shell");
var fc;
var s="";

fso = WScript.CreateObject("Scripting.FileSystemObject");
tf = fso.CreateTextFile("out.html");
tf.writeline("<!doctype html>");
tf.writeline("<html>");
tf.writeline("<body>");

fc = new Enumerator(fso.GetFolder(".").Files);
for (;!fc.atEnd();fc.moveNext())
{
	s = fso.GetFileName(String(fc.item()));
	if (s.search("html")>0)
		tf.writeline("<p><a href = \"" + s + "\">" + s + "</a></p>");
}

tf.writeline("</body>");
tf.writeline("</html>");
tf.Close();
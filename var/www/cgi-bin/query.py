#!/usr/bin/python3
import cgi

print("Content-Type: text/html\n")
print("<h2>Query Parameters:</h2>")
print("<pre>")
form = cgi.FieldStorage(keep_blank_values=True)
for key in form.keys():
    print(f"{key} : {form[key].value}")
print("</pre>")

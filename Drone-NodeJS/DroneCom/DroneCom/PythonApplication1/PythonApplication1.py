from Naked.toolshed.shell import execute_js

print "calling flightplan.js"
success = execute_js('testscript.js')
if success:
    print "sucess"
else:
    print "error"

# Loads mkmf which is used to make makefiles for Ruby extensions
require 'mkmf'

CONFIG['LDSHARED'] = "g++ -shared"

# Give it a name
extension_name = 'cproctable'

# The destination
dir_config(extension_name)

# Do the work
create_makefile(extension_name)


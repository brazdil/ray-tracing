env = Environment()

# Predefined functions 

def CheckPKGConfig(context, version):
	context.Message( 'Checking for pkg-config... ' )
	ret = context.TryAction('pkg-config --atleast-pkgconfig-version=%s' % version)[0]
	context.Result( ret )
	return ret
 
def CheckPKG(context, name):
	context.Message( 'Checking for %s... ' % name )
	ret = context.TryAction('pkg-config --exists \'%s\'' % name)[0]
	context.Result( ret )
	return ret

# Configuration:

conf = Configure(env, custom_tests = { 'CheckPKGConfig' : CheckPKGConfig,
                                       'CheckPKG' : CheckPKG })

if not conf.CheckPKGConfig('0.15.0'):
	print 'pkg-config >= 0.15.0 not found.'
	Exit(1)

if not conf.CheckPKG('sdl >= 1.2.14'):
	print 'libsdl >= 1.2.14 not found.'
	Exit(1)

if not conf.CheckPKG('eigen3 >= 3.0.5'):
	print 'eigen3 >= 3.0.5 not found.'
	Exit(1)

# Build

compile_append = ' -g '

boost_dir = ARGUMENTS.get('boost-dir', '')
if boost_dir:
	compile_append += '-I' + boost_dir

env.ParseConfig('pkg-config --cflags eigen3')

source_all = [ file.rstr() for file in Glob('src/*.cpp') ]
source_common = [ filename for filename in source_all 
                  if (not 'src/main_' in filename) and 
                     (not '_test' in filename) and
                     (not 'src/Master.cpp' in filename) and
                     (not 'src/Slave.cpp' in filename) and
                     (not 'src/ResultManager.cpp' in filename) and
                     (not 'src/SDL_rotozoom.c' in filename) ]
source_master = ['src/main_master.cpp', 'src/Master.cpp', 'src/ResultManager.cpp', 'src/SDL_rotozoom.c']
source_slave = ['src/main_slave.cpp', 'src/Slave.cpp']

includes_master = compile_append + ' `pkg-config --cflags sdl` '
includes_slave = compile_append + '';

name_lib_common = 'raytracing-shared'
libs_common = [name_lib_common, 'archive', 'boost_system', 'boost_thread', 'boost_program_options'];
libs_master = libs_common + ['SDL']
libs_slave = libs_common

env.SharedLibrary(name_lib_common, source_common)
program_slave = env.Program('slave', source_slave, LIBS = libs_slave, CCFLAGS = includes_slave, LIBPATH = '.')
program_master = env.Program('master', source_master, LIBS = libs_master, CCFLAGS = includes_master, LIBPATH = '.')


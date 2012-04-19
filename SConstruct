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

compile_append = '  '

# boost_dir = ARGUMENTS.get('boost-dir', '')
# if boost_dir:
# 	compile_append += '-I' + boost_dir

env.ParseConfig('pkg-config --cflags eigen3')

source_common = Glob('src/common/*.cpp')
source_master = Glob('src/master/*.cpp') + Glob('src/master/*.c')
source_slave = Glob('src/slave/*.cpp')
source_easybmp = Glob('src/easybmp/*.cpp')
source_sdl_gfx = Glob('src/sdl_gfx/*.c')

includes_sdl = ' `pkg-config --cflags sdl` '
includes_sdl_gfx = includes_sdl;
includes_common = compile_append;
includes_master = compile_append + includes_sdl
includes_slave = compile_append;

name_lib_common = 'raytracing-shared'
name_lib_easybmp = 'easybmp'
name_lib_sdl_gfx = 'sdl_gfx'

libs_common = [name_lib_common, 'archive', 'boost_system', 'boost_thread', 'boost_program_options'];
libs_master = libs_common + ['SDL', name_lib_easybmp, name_lib_sdl_gfx]
libs_slave = libs_common

env.StaticLibrary(name_lib_easybmp, source_easybmp)
env.StaticLibrary(name_lib_sdl_gfx, source_sdl_gfx, CCFLAGS = includes_sdl_gfx)
env.StaticLibrary(name_lib_common, source_common, CCFLAGS = includes_common)

program_slave = env.Program('slave', source_slave, LIBS = libs_slave, CCFLAGS = includes_slave, LIBPATH = '.')
program_master = env.Program('master', source_master, LIBS = libs_master, CCFLAGS = includes_master, LIBPATH = '.')


env = Environment()

env.ParseConfig('pkg-config --cflags eigen3')
env.Append(LIBS = ['archive', 'boost_system', 'boost_thread', 'boost_program_options'])

boost_dir = ARGUMENTS.get('boost-dir', '')
if boost_dir:
	env.Append(CCFLAGS = '-I' + boost_dir)
	
source_all = [ file.rstr() for file in Glob('src/*.cpp') ]
source_common = [ filename for filename in source_all if (not 'src/main_' in filename) and (not '_test' in filename) ]
source_master = ['src/main_master.cpp'] + source_common
source_slave = ['src/main_slave.cpp'] + source_common

program_master = env.Program('master', source_master)
program_slave = env.Program('slave', source_slave)

Default(program_master)

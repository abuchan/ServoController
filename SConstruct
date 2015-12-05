# gnu arm toolchain must be already in system path

import os
env = Environment(ENV = os.environ)

env['AR'] = 'arm-none-eabi-ar'
env['AS'] = 'arm-none-eabi-as'
env['CC'] = 'arm-none-eabi-gcc'
env['CXX'] = 'arm-none-eabi-g++'
env['LINK'] = 'arm-none-eabi-g++'                # predefined is 'arm-none-eabi-gcc'
env['RANLIB'] = 'arm-none-eabi-ranlib'
env['OBJCOPY'] = 'arm-none-eabi-objcopy'
env['PROGSUFFIX'] = '.elf'

cpu = 'cortex-m0plus'
# compiler flags
env.Append(CCFLAGS = [
    '-mcpu=' + cpu,
    '-mthumb',
    '-c',
    '-g',
    '-Wall', '-Wextra',
    '-fno-common',
    '-fno-exceptions',
    '-fmessage-length=0',
    '-ffunction-sections',
    '-fdata-sections',
    '-fomit-frame-pointer',
    '-MMD',
    '-MP',
    '-DNDEBUG',
    '-Os',
    ])
env.Append(CFLAGS = [
    '-std=gnu99',
    ])
env.Append(CXXFLAGS = [
    '-std=gnu++98',
    '-fno-rtti',
    ])


linker_script = 'mbed-src/targets/cmsis/TARGET_Freescale/TARGET_KLXX/TARGET_KL05Z/TOOLCHAIN_GCC_ARM/MKL05Z4.ld'
# linker flags
env.Append(LINKFLAGS = [
    '-mcpu=' + cpu,
    '-mthumb',
    '-T' + linker_script,
    '-Wl,--gc-sections',
    '-Wl,--wrap,main',
    '-Wl,-Map=main.map,--cref',
    '--specs=nano.specs',

    '-lstdc++',
    '-lsupc++',
    '-lm',
    '-lc',
    '-lgcc',
    '-lnosys',
    ])

# defines
env.Append(CPPDEFINES = [
    'TARGET_FF_ARDUINO',
    'TOOLCHAIN_GCC_ARM',
    'TARGET_KLXX',
    'TARGET_KL05Z',
    'TARGET_CORTEX_M',
    'TARGET_M0P',
    'TARGET_Freescale',
    'MBED_BUILD_TIMESTAMP=1448404265.7',
    '__CORTEX_M0PLUS',
    'TOOLCHAIN_GCC',
    '__MBED__=1',
    'ARM_MATH_CM0PLUS',
])

kl05z_cmsis_base = 'mbed-src/targets/cmsis/TARGET_Freescale/TARGET_KLXX/TARGET_KL05Z/'
kl05z_cmsis_sources = Glob(kl05z_cmsis_base + '*.c') + Glob(kl05z_cmsis_base + "TOOLCHAIN_GCC_ARM/" + '*.S')

kl05z_hal_base = 'mbed-src/targets/hal/TARGET_Freescale/TARGET_KLXX/TARGET_KL05Z/'
kl05z_hal_sources = Glob(kl05z_hal_base + '*.c')

klxx_hal_base = 'mbed-src/targets/hal/TARGET_Freescale/TARGET_KLXX/'
klxx_hal_sources = Glob(klxx_hal_base + '*.c')

mbed_base = 'mbed-src/common/'
mbed_sources = Glob(mbed_base + '*.c') + Glob(mbed_base + '*.cpp')

modserial_base = 'MODSERIAL/'
modserial_sources = Glob(modserial_base + '*.cpp') + [modserial_base + 'Device/MODSERIAL_KL05Z.cpp']

telemetry_base = 'telemetry/server-cpp/'
telemetry_sources = Glob(telemetry_base + '*.cpp')

top_sources = [
    'PID.cpp',
    'Encoder.cpp',
    'Motor.cpp',
    'CurrentSense.cpp',
    'HBridge.cpp',
    'QEI/QEI.cpp',

    'CPU16.cpp',
    ]

# include locations
env['CPPPATH'] = [
    'mbed-src/api/',
    'mbed-src/common/',
    'mbed-src/hal/',
    'mbed-src/targets/cmsis',
    'mbed-src/targets/cmsis/TARGET_Freescale/TARGET_KLXX/TARGET_KL05Z',
    'mbed-src/targets/hal/TARGET_Freescale/TARGET_KLXX',
    'mbed-src/targets/hal/TARGET_Freescale/TARGET_KLXX/TARGET_KL05Z',
    'QEI',
    modserial_base, modserial_base + 'Device/',
    telemetry_base,
    ]

# build everything
prg = env.Program(
    target = 'main',
    source = mbed_sources + klxx_hal_sources + kl05z_cmsis_sources + kl05z_hal_sources + modserial_sources + telemetry_sources + top_sources
)

# binary file builder
def arm_generator(source, target, env, for_signature):
    return '$OBJCOPY -O binary %s %s'%(source[0], target[0])
env.Append(BUILDERS = {
    'Objcopy': Builder(
        generator=arm_generator,
        suffix='.bin',
        src_suffix='.elf'
    )
})

env.Objcopy(prg)

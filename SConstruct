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
kl05z_cmsis_sources = [
    'TOOLCHAIN_GCC_ARM/startup_MKL05Z4.S',
    'cmsis_nvic.c',
    'system_MKL05Z4.c',
    ]
kl05z_cmsis_sources = [kl05z_cmsis_base + x for x in kl05z_cmsis_sources]

kl05z_hal_base = 'mbed-src/targets/hal/TARGET_Freescale/TARGET_KLXX/TARGET_KL05Z/'
kl05z_hal_sources = [
    'PeripheralPins.c',
    'gpio_irq_api.c',
    'mbed_overrides.c',
    'serial_api.c',
    'spi_api.c',
    ]
kl05z_hal_sources = [kl05z_hal_base + x for x in kl05z_hal_sources]

klxx_hal_base = 'mbed-src/targets/hal/TARGET_Freescale/TARGET_KLXX/'
klxx_hal_sources = [
    'analogin_api.c',
    'analogout_api.c',
    'gpio_api.c',
    'i2c_api.c',
    'pinmap.c',
    'port_api.c',
    'pwmout_api.c',
    'rtc_api.c',
    'sleep.c',
    'us_ticker.c',
    ]
klxx_hal_sources = [klxx_hal_base + x for x in klxx_hal_sources]

mbed_base = 'mbed-src/common/'
mbed_sources = ['assert.c',
    'board.c',
    'error.c',
    'gpio.c',
    'lp_ticker_api.c',
    'mbed_interface.c',
    'pinmap_common.c',
    'rtc_time.c',
    'semihost_api.c',
    'ticker_api.c',
    'us_ticker_api.c',
    'wait_api.c',
    'BusIn.cpp',
    'BusInOut.cpp',
    'BusOut.cpp',
    'CAN.cpp',
    'CallChain.cpp',
    'Ethernet.cpp',
    'FileBase.cpp',
    'FileLike.cpp',
    'FilePath.cpp',
    'FileSystemLike.cpp',
    'I2C.cpp',
    'I2CSlave.cpp',
    'InterruptIn.cpp',
    'InterruptManager.cpp',
    'LocalFileSystem.cpp',
    'RawSerial.cpp',
    'SPI.cpp',
    'SPISlave.cpp',
    'Serial.cpp',
    'SerialBase.cpp',
    'Stream.cpp',
    'Ticker.cpp',
    'Timeout.cpp',
    'Timer.cpp',
    'TimerEvent.cpp',
    'retarget.cpp',
    ]
mbed_sources = [mbed_base + x for x in mbed_sources]

modserial_base = 'MODSERIAL/'
modserial_sources = [
    'FLUSH.cpp',
    'GETC.cpp',
    'INIT.cpp',
    'ISR_RX.cpp',
    'ISR_TX.cpp',
    'MODSERIAL.cpp',
    'MODSERIAL_IRQ_INFO.cpp',
    'PUTC.cpp',
    'RESIZE.cpp',
    'Device/MODSERIAL_KL05Z.cpp',
    ]
modserial_sources = [modserial_base + x for x in modserial_sources]

telemetry_base = 'telemetry/server-cpp/'
telemetry_sources = [
    'telemetry.cpp',
    'telemetry-data.cpp',
    'telemetry-mbed-hal.cpp',
    ]
telemetry_sources = [telemetry_base + x for x in telemetry_sources]

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

# Top-level build wrapper so build outputs go in a separate directory.
import multiprocessing
SetOption('num_jobs', multiprocessing.cpu_count() + 1)

SConscript('SConscript', variant_dir='build')

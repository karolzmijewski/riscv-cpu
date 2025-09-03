import stest
import info_status
import simics
import riscv_cpu_common

info_status.check_for_info_status(["riscv-cpu"])
dev = riscv_cpu_common.create_riscv_cpu()

for obj in [dev]:
    for cmd in ["info", "status"]:
        try:
            simics.SIM_run_command(obj.name + "." + cmd)
        except simics.SimExc_General as e:
            stest.fail(cmd + " command failed: " + str(e))

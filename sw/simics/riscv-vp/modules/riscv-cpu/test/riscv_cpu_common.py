import simics

def create_riscv_cpu(name = None):
    """
    Create a new riscv_cpu object
    """
    riscv_cpu = simics.pre_conf_object(name, "riscv_cpu")
    simics.SIM_add_configuration([riscv_cpu], None)
    return simics.SIM_get_object(riscv_cpu.name)

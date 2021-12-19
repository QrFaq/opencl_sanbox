import subprocess
import re
import matplotlib.pyplot as plt
import numpy as np

def get_value_param_from_log(output, line_start):
    value = None
    for line in output.split("\n"):
        if line_start in line:
            value_str = re.findall(r"[-+]?\d*\.\d+|\d+", line)[0]
            value = float(value_str)
            break
    return value


exec_path = r"/full/path/opencl_sanbox/sha256_opencl_load_test/build/main"
kernel_fpath = r"/full/path/opencl_sanbox/sha256_opencl/src/sha256.cl"
save_fig_fpath = r"/full/path/opencl_sanbox/sha256_opencl_load_test/build"

test_time = 10
GPU_PARAMS = {
    "DEVICE-MAX-WG": 24,
    "KERNEL-WG-SZ": 24
}



###
# Log collection of kernel launch
# parameters for graphs drawing
# by combinatorial checking
# `global`` to `local` pairs
###
kernel_test_info = []
for global_work_size in range(1, GPU_PARAMS["DEVICE-MAX-WG"]+1):
    for local_work_size in range(1, GPU_PARAMS["KERNEL-WG-SZ"]+1):
        # command generation
        cmd_args = f"--k {kernel_fpath} --t {test_time} --g {global_work_size} --l {local_work_size}"
        cmd = f"{exec_path} {cmd_args}"
        print(f"> Launching, time={test_time} [s]:")
        print(f"\tglobal_work_size={global_work_size} from {GPU_PARAMS['DEVICE-MAX-WG']}, local_work_size={local_work_size} from {GPU_PARAMS['KERNEL-WG-SZ']}")

        # if global divisible by local, then launch test
        if global_work_size % local_work_size == 0:
            cmd_output = subprocess.run([
                exec_path,
                f"--k", f"{kernel_fpath}",
                f"--t", f"{test_time}",
                f"--g", f"{global_work_size}",
                f"--l", f"{local_work_size}"
            ], capture_output=True, text=True).stdout
            speed = get_value_param_from_log(cmd_output, line_start="\tKernel calculation time per batch: ")
            kernel_test_info.append((global_work_size, local_work_size, speed))

kernel_test_info = np.array(kernel_test_info)
np.save("kernel_test_info", kernel_test_info)


fig1, ax1 = plt.subplots()
line1, = ax1.plot(kernel_test_info[:, 0], kernel_test_info[:, 2], "--o", color="b", label="Speed(global wsz)")
line2, = ax1.plot(kernel_test_info[:, 1], kernel_test_info[:, 2], "-*", color="r", alpha=0.3, label="Speed(local wsz)")
ax1.set_ylabel('Speed,[hash/s]')
ax1.set_xlabel('global OR local wsz, conv. units')
ax1.set_title(f"Kernel test Speed(global OR local work size), time={test_time} [s]")
plt.legend()
plt.savefig(f"{save_fig_fpath}/speed.png")
# plt.show()

# create heat-map visualization matrix
kernel_test_info_mx = np.zeros((GPU_PARAMS["DEVICE-MAX-WG"], GPU_PARAMS["KERNEL-WG-SZ"]))
for row_ind in range(0, kernel_test_info.shape[0]):
    r, c, speed = kernel_test_info[row_ind, :]
    r-=1
    c-=1
    kernel_test_info_mx[int(r), int(c)] = speed

fig2, ax2 = plt.subplots()
plt_img = ax2.matshow(kernel_test_info_mx)
divider = make_axes_locatable(ax2)
cax = divider.append_axes("right", size="5%", pad=0.05)
fig2.colorbar(plt_img, cax=cax,ax=ax2)
ax2.xaxis.set_ticks_position('bottom')
ax2.yaxis.set_ticks_position('left')
ax2.set_ylabel('global work size')
ax2.set_xlabel('local work size')
ax2.set_title(f"Heat map Speed(global, local) work size, time={test_time} [s]")
plt.savefig(f"{save_fig_fpath}/heat_map.png")
#plt.show()
plt.close()
#!/usr/bin/env python3

# Formula found in Appendix B of the AD2420 Technical Reference Manual
# Link: https://www.analog.com/media/en/technical-documentation/user-guides/AD242x_TRM_Rev1.1.pdf 

respoffs_master = 248

dnslot_activity = 0
upslot_activity = list(map(lambda x: (15-x) * 25, range(16)))

respcycs_up = []
respcycs_dn = []
for i in range(16):
    respcycs_dn.append(((64+dnslot_activity) // 4) + 4 * i + 2)
    respcycs_up.append(respoffs_master - (((64 + upslot_activity[i]) // 4) + 1))

assert(max(respcycs_dn) <= min(respcycs_up))

print(respcycs_up)
print(respcycs_dn)

print(f"resp_master = {(max(respcycs_dn) + min(respcycs_up)) // 2}")

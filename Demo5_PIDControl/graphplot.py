#-------------------------------------------------------------------------------
# Name:        Graph plot
# Purpose:
#
# Author:      Steven Song
#
# Created:     10/04/2024
# Copyright:   (c) Steven Song 2024
# Licence:     <your licence>
#-------------------------------------------------------------------------------

import matplotlib.pyplot as plt
import re

content = """
  0.0s> L    0/ 0(+128 R    0/ 0(+128 #8.0 8.0 0.0
  0.1s> L    4/ 4(+112 R    4/ 4(+112 #12.0 12.0 0.0
  0.2s> L    9/ 5(+120 R   10/ 6(+104 #15.0 14.0 0.0
  0.3s> L   16/ 7(+106 R   17/ 7(+100 #16.0 15.0 0.0
  0.4s> L   22/ 6(+128 R   24/ 7(+106 #18.0 16.0 0.0
  0.5s> L   30/ 8(+108 R   31/ 7(+112 #18.0 17.0 0.0
  0.6s> L   37/ 7(+124 R   39/ 8(+102 #19.0 17.0 0.0
  0.7s> L   45/ 8(+114 R   47/ 8(+102 #19.0 17.0 0.0
  0.8s> L   53/ 8(+114 R   55/ 8(+102 #19.0 17.0 0.0
  0.9s> L   61/ 8(+114 R   64/ 9(+86 #19.0 16.0 0.0
  1.0s> L   69/ 8(+114 R   72/ 8(+96 #19.0 16.0 0.0
  1.1s> L   77/ 8(+114 R   80/ 8(+96 #19.0 16.0 0.0
  1.2s> L   85/ 8(+114 R   88/ 8(+96 #19.0 16.0 0.0
  1.3s> L   94/ 9(+98 R   96/ 8(+96 #18.0 16.0 0.0
  1.4s> L  102/ 8(+108 R  104/ 8(+96 #18.0 16.0 0.0
  1.5s> L  110/ 8(+108 R  113/ 9(+80 #18.0 15.0 0.0
  1.6s> L  118/ 8(+108 R  120/ 7(+106 #18.0 16.0 0.0
  1.7s> L  126/ 8(+108 R  129/ 9(+80 #18.0 15.0 0.0
  1.8s> L  135/ 9(+92 R  137/ 8(+90 #17.0 15.0 0.0
  1.9s> L  143/ 8(+102 R  145/ 8(+90 #17.0 15.0 0.0
  2.0s> L  150/ 7(+118 R  153/ 8(+90 #18.0 15.0 0.0
  2.1s> L  158/ 8(+108 R  161/ 8(+90 #18.0 15.0 0.0
  2.2s> L  166/ 8(+108 R  168/ 7(+106 #18.0 16.0 0.0
  2.3s> L  175/ 9(+92 R  176/ 8(+96 #17.0 16.0 0.0
  2.4s> L  183/ 8(+102 R  185/ 9(+80 #17.0 15.0 0.0
  2.5s> L  191/ 8(+102 R  193/ 8(+90 #17.0 15.0 0.0
  2.6s> L  199/ 8(+102 R  201/ 8(+90 #17.0 15.0 0.0
  2.7s> L  207/ 8(+102 R  209/ 8(+90 #17.0 15.0 0.0
  2.8s> L  215/ 8(+102 R  217/ 8(+90 #17.0 15.0 0.0
  2.9s> L  223/ 8(+102 R  225/ 8(+90 #17.0 15.0 0.0
  3.0s> L  232/ 9(+86 R  233/ 8(+90 #16.0 15.0 0.0
  3.1s> L  240/ 8(+96 R  241/ 8(+90 #16.0 15.0 0.0
  3.2s> L  248/ 8(+96 R  249/ 8(+90 #16.0 15.0 0.0
  3.3s> L  254/ 6(+128 R  256/ 7(+106 #18.0 16.0 0.0
  3.4s> L  262/ 8(+108 R  263/ 7(+112 #18.0 17.0 0.0
  3.5s> L  269/ 7(+124 R  271/ 8(+102 #19.0 17.0 0.0
  3.6s> L  278/ 9(+98 R  280/ 9(+86 #18.0 16.0 0.0
  3.7s> L  286/ 8(+108 R  288/ 8(+96 #18.0 16.0 0.0
  3.8s> L  294/ 8(+108 R  297/ 9(+80 #18.0 15.0 0.0
  3.9s> L  303/ 9(+92 R  305/ 8(+90 #17.0 15.0 0.0
  4.0s> L  311/ 8(+102 R  314/ 9(+74 #17.0 14.0 0.0
"""
#1       |10       |20       |30       |40       |50
t_list = []
l_speed_list = []
l_encoder_list = []
r_speed_list = []
r_encoder_list = []

for i in content.split("\n"):
    if not i.strip(): continue
    t = float(i[:5].strip())
    l_odom = float(i[9:14].strip())
    l_speed = float(i[15:17].strip())

    r_odom = float(i.split("R")[1][0:5].strip())
    r_speed = float(i.split("R")[1][6:8].strip())

    t_list.append(t)
    l_encoder_list.append(l_odom)
    l_speed_list.append(l_speed)
    r_encoder_list.append(r_odom)
    r_speed_list.append(r_speed)

fig, ax = plt.subplots()
ax.plot(t_list, l_speed_list, label='Left Wheel', marker='o')
ax.plot(t_list, r_speed_list, label='Right Wheel', marker='o')
ax.set_xlabel('Time (s)')
ax.set_ylabel('Speed (counts/100ms)')
ax.set_title('Time-Counting Diagram')
ax.legend()
plt.show()

fig, ax = plt.subplots()
ax.plot(t_list, l_encoder_list, label='Left Wheel', marker='o')
ax.plot(t_list, r_encoder_list, label='Right Wheel', marker='o')
ax.set_xlabel('Time (s)')
ax.set_ylabel('Distance (counts)')
ax.set_title('Time-Counting Diagram')
ax.legend()
plt.show()


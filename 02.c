Since PC0 and PC1 sit behind different routers, they can't share one /24. With a /24 mask, PC1 would think PC0 is on its own LAN and never send traffic to the gateway. The fix is to split 192.168.1.0/24 with a /28 mask (255.255.255.240), which keeps your addresses:

- PC0: `192.168.1.10`, mask `255.255.255.240`, gateway `192.168.1.1`
- PC1: `192.168.1.20`, mask `255.255.255.240`, gateway `192.168.1.17`

**Router0**
```
enable
configure terminal
hostname Router0
interface gigabitEthernet0/0
 ip address 192.168.1.1 255.255.255.240
 no shutdown
interface gigabitEthernet0/1
 ip address 10.0.0.1 255.255.255.252
 no shutdown
exit
ip route 10.0.0.4 255.255.255.252 10.0.0.2
ip route 192.168.1.16 255.255.255.240 10.0.0.2
end
write memory
```

**Router1**
```
enable
configure terminal
hostname Router1
interface gigabitEthernet0/0
 ip address 10.0.0.2 255.255.255.252
 no shutdown
interface gigabitEthernet0/1
 ip address 10.0.0.5 255.255.255.252
 no shutdown
exit
ip route 192.168.1.0 255.255.255.240 10.0.0.1
ip route 192.168.1.16 255.255.255.240 10.0.0.6
end
write memory
```

**Router2**
```
enable
configure terminal
hostname Router2
interface gigabitEthernet0/0
 ip address 192.168.1.17 255.255.255.240
 no shutdown
interface gigabitEthernet0/1
 ip address 10.0.0.6 255.255.255.252
 no shutdown
exit
ip route 10.0.0.0 255.255.255.252 10.0.0.5
ip route 192.168.1.0 255.255.255.240 10.0.0.5
end
write memory
```

**Test** (from PC0 command prompt)
```
ping 192.168.1.20
```

If your teacher requires the same /24 on both PCs, that only works with proxy ARP or bridging instead of normal routing. Tell me if that's the case and I'll adjust it.

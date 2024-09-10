# RBF_Packages
2024/03/01 Initial commit  
This is a Maya dependecy node developed by Maya C++ API  
The function is use to calculate the suitable weight arrary according to the RBF algorithm  
To use this node, please download the RBF_Solver_Node.mll and place it in your maya plugin folder. Then load this node in your maya plugin manager  

2024/09/10 updated version 1.1  
Combine Two Nodes(RBF_Solver and WeightMultiply) into one plugin, now you only need to load RBF_Packages.mll then you can use two nodes  
Project updated: now the project will carry with lib and all will use local path which means there's no extra settings before you build the plugin, so it's easy to do your modifications  
Nodes updated: adjust the input of RBF_Nodes，and two nodes can automaticly delete the unused attributes now.  

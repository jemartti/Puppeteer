--
-- CS488 -- Introduction to Computer Graphics
-- 
-- puppet.lua


-- Define our colours library
aquamarine = gr.material({0.5, 1.0, 0.8}, {0.1, 0.1, 0.1}, 10)
blue = gr.material({0.0, 0.0, 1.0}, {0.1, 0.1, 0.1}, 10)
red = gr.material({1.0, 0.0, 0.0}, {0.1, 0.1, 0.1}, 10)


-- Root Node
rootnode = gr.node('root')
rootnode:translate(-2.0, 3.0, -10.0)
rootnode:scale(0.6, 0.6, 0.6)
rootnode:rotate('y', -10)


-- Torso
Torso = gr.sphere('Torso')
rootnode:add_child(Torso)
Torso:set_material(aquamarine)
Torso:translate(0.0, 0.0, -18)
Torso:scale(1.0, 1.5, 0.7)


-- Shoulders
Shoulders = gr.sphere('Shoulders')
rootnode:add_child(Shoulders)
Shoulders:set_material(aquamarine)
Shoulders:translate(0.0, 1.5, -18)
Shoulders:scale(1.5, 0.5, 0.7)
ShouldersJoint = gr.joint('ShouldersJoint', {-5.0, 0.0, 5.0}, {0.0, 0.0, 0.0})
Shoulders:add_child(ShouldersJoint)
ShouldersJoint:rotate('x', -45)
ShouldersE = gr.node('ShouldersE')
rootnode:add_child(ShouldersE)
ShouldersE:translate(0.0, 1.5, -18)


-- Left Upper Arm
LeftUpperArmJoint = gr.joint('LeftUpperArmJoint', {-90.0, 0.0, 40.0}, {0.0, 0.0, 0.0})
ShouldersE:add_child(LeftUpperArmJoint)
LeftUpperArmJoint:translate(-1.4, 0.15, 0.0)
LeftUpperArm = gr.sphere('LeftUpperArm')
LeftUpperArmJoint:add_child(LeftUpperArm)
LeftUpperArm:set_material(blue)
LeftUpperArm:translate(0.0, -1.25, 0.0)
LeftUpperArm:scale(0.3, 1.25, 0.3)
LeftUpperArmE = gr.node('LeftUpperArmE')
LeftUpperArmJoint:add_child(LeftUpperArmE)
LeftUpperArmE:translate(0.0, -1.25, 0.0)

-- Left Forearm
LeftForearmJoint = gr.joint('LeftForearmJoint', {-60.0, 0.0, 0.0}, {0.0, 0.0, 0.0})
LeftUpperArmE:add_child(LeftForearmJoint)
LeftForearmJoint:translate(0.0, -1.05, 0.0)
LeftForearm = gr.sphere('LeftForearm')
LeftForearmJoint:add_child(LeftForearm)
LeftForearm:set_material(red)
LeftForearm:translate(0.0, -0.75, 0.0)
LeftForearm:scale(0.3, 0.75, 0.3)
LeftForearmE = gr.node('LeftForearmE')
LeftForearmJoint:add_child(LeftForearmE)
LeftForearmE:translate(0.0, -0.75, 0.0)

-- Left Hand
LeftHandJoint = gr.joint('LeftHandJoint', {-30.0, 0.0, 40}, {0.0, 0.0, 0.0})
LeftForearmE:add_child(LeftHandJoint)
LeftHandJoint:translate(0.0, -0.65, 0.0)
LeftHand = gr.sphere('LeftHand')
LeftHandJoint:add_child(LeftHand)
LeftHand:set_material(blue)
LeftHand:translate(0.0, -0.3, 0.0)
LeftHand:scale(0.3, 0.3, 0.3)


-- Neck
NeckJoint = gr.joint('NeckJoint', {-5.0, 0.0, 5.0}, {0.0, 0.0, 0.0})
ShouldersE:add_child(NeckJoint)
NeckJoint:translate(0.0, 0.1, 0.0)
Neck = gr.sphere('Neck')
NeckJoint:add_child(Neck)
Neck:set_material(blue)
Neck:translate(0, 0.5, 0)
Neck:scale(0.3, 0.5, 0.3)
NeckE = gr.node('NeckE')
NeckJoint:add_child(NeckE)
NeckE:translate(0.0, 0.5, 0.0)

--Head
HeadJoint = gr.joint('HeadJoint', {0.0, 0.0, 0.0}, {-10.0, 0.0, 10.0})
NeckE:add_child(HeadJoint)
HeadJoint:translate(0.0, 0.1, 0.0)
Head = gr.sphere('Head')
HeadJoint:add_child(Head)
Head:set_material(aquamarine)
Head:translate(0, 0.5, 0)
Head:scale(0.5, 0.5, 0.5)
HeadE = gr.node('HeadE')
HeadJoint:add_child(HeadE)
HeadE:translate(0.0, 0.5, 0.0)

-- Nose
Nose = gr.sphere('Nose')
Head:add_child(Nose)
Nose:set_material(blue)
Nose:translate(0.0, 0.0, 0.5)
Nose:scale(0.1, 0.1, 1.0)


-- Right Upper Arm
RightUpperArmJoint = gr.joint('RightUpperArmJoint', {-90.0, 0.0, 40.0}, {0.0, 0.0, 0.0})
ShouldersE:add_child(RightUpperArmJoint)
RightUpperArmJoint:translate(1.4, 0.15, 0.0)
RightUpperArm = gr.sphere('RightUpperArm')
RightUpperArmJoint:add_child(RightUpperArm)
RightUpperArm:set_material(blue)
RightUpperArm:translate(0.0, -1.25, 0.0)
RightUpperArm:scale(0.3, 1.25, 0.3)
RightUpperArmE = gr.node('RightUpperArmE')
RightUpperArmJoint:add_child(RightUpperArmE)
RightUpperArmE:translate(0.0, -1.25, 0.0)

-- Right Forearm
RightForearmJoint = gr.joint('RightForearmJoint', {-60.0, 0.0, 0.0}, {0.0, 0.0, 0.0})
RightUpperArmE:add_child(RightForearmJoint)
RightForearmJoint:translate(0.0, -1.05, 0.0)
RightForearm = gr.sphere('RightForearm')
RightForearmJoint:add_child(RightForearm)
RightForearm:set_material(red)
RightForearm:translate(0.0, -0.75, 0.0)
RightForearm:scale(0.3, 0.75, 0.3)
RightForearmE = gr.node('RightForearmE')
RightForearmJoint:add_child(RightForearmE)
RightForearmE:translate(0.0, -0.75, 0.0)

-- Right Hand
RightHandJoint = gr.joint('RightHandJoint', {-30.0, 0.0, 40.0}, {0.0, 0.0, 0.0})
RightForearmE:add_child(RightHandJoint)
RightHandJoint:translate(0.0, -0.65, 0.0)
RightHand = gr.sphere('RightHand')
RightHandJoint:add_child(RightHand)
RightHand:set_material(blue)
RightHand:translate(0.0, -0.3, 0.0)
RightHand:scale(0.3, 0.3, 0.3)


-- Hips
Hips = gr.sphere('Hips')
rootnode:add_child(Hips)
Hips:set_material(blue)
Hips:translate(0.0, -1.5, -18)
Hips:scale(0.9, 0.5, 0.7)
HipsE = gr.node('HipsE')
rootnode:add_child(HipsE)
HipsE:translate(0.0, -1.5, -18)


-- Left Thigh
LeftUpperLegJoint = gr.joint('LeftUpperLegJoint', {-45.0, 0.0, 40.0}, {0.0, 0.0, 0.0})
HipsE:add_child(LeftUpperLegJoint)
LeftUpperLegJoint:translate(-0.6, 0.1, 0.0)
LeftUpperLeg = gr.sphere('LeftUpperLeg')
LeftUpperLegJoint:add_child(LeftUpperLeg)
LeftUpperLeg:set_material(blue)
LeftUpperLeg:translate(0.0, -1.25, 0.0)
LeftUpperLeg:scale(0.5, 1.25, 0.5)
LeftUpperLegE = gr.node('LeftUpperLegE')
LeftUpperLegJoint:add_child(LeftUpperLegE)
LeftUpperLegE:translate(0.0, -1.25, 0.0)

-- Left Calf
LeftCalfJoint = gr.joint('LeftCalfJoint', {0.0, 0.0, 45.0}, {0.0, 0.0, 0.0})
LeftUpperLegE:add_child(LeftCalfJoint)
LeftCalfJoint:translate(0.0, -1.05, 0.0)
LeftCalf = gr.sphere('LeftCalf')
LeftCalfJoint:add_child(LeftCalf)
LeftCalf:set_material(red)
LeftCalf:translate(0.0, -1.25, 0.0)
LeftCalf:scale(0.3, 1.25, 0.3)
LeftCalfE = gr.node('LeftCalfE')
LeftCalfJoint:add_child(LeftCalfE)
LeftCalfE:translate(0.0, -1.25, 0.0)

-- Left Foot
LeftFootJoint = gr.joint('LeftFootJoint', {0.0, 0.0, 50}, {0.0, 0.0, 0.0})
LeftCalfE:add_child(LeftFootJoint)
LeftFootJoint:translate(0.0, -1.15, 0.0)
LeftFoot = gr.sphere('LeftFoot')
LeftFootJoint:add_child(LeftFoot)
LeftFoot:set_material(blue)
LeftFoot:translate(0.0, -0.2, 0.0)
LeftFoot:scale(0.3, 0.2, 0.4)


-- Right Thigh
RightUpperLegJoint = gr.joint('RightUpperLegJoint', {-45.0, 0.0, 40.0}, {0.0, 0.0, 0.0})
HipsE:add_child(RightUpperLegJoint)
RightUpperLegJoint:translate(0.6, 0.1, 0.0)
RightUpperLeg = gr.sphere('RightUpperLeg')
RightUpperLegJoint:add_child(RightUpperLeg)
RightUpperLeg:set_material(blue)
RightUpperLeg:translate(0.0, -1.25, 0.0)
RightUpperLeg:scale(0.5, 1.25, 0.5)
RightUpperLegE = gr.node('RightUpperLegE')
RightUpperLegJoint:add_child(RightUpperLegE)
RightUpperLegE:translate(0.0, -1.25, 0.0)

-- Right Calf
RightCalfJoint = gr.joint('RightCalfJoint', {0.0, 0.0, 45.0}, {0.0, 0.0, 0.0})
RightUpperLegE:add_child(RightCalfJoint)
RightCalfJoint:translate(0.0, -1.05, 0.0)
RightCalf = gr.sphere('RightCalf')
RightCalfJoint:add_child(RightCalf)
RightCalf:set_material(red)
RightCalf:translate(0.0, -1.25, 0.0)
RightCalf:scale(0.3, 1.25, 0.3)
RightCalfE = gr.node('RightCalfE')
RightCalfJoint:add_child(RightCalfE)
RightCalfE:translate(0.0, -1.25, 0.0)

-- Right Foot
RightFootJoint = gr.joint('RightFootJoint', {0.0, 0.0, 50.0}, {0.0, 0.0, 0.0})
RightCalfE:add_child(RightFootJoint)
RightFootJoint:translate(0.0, -1.15, 0.0)
RightFoot = gr.sphere('RightFoot')
RightFootJoint:add_child(RightFoot)
RightFoot:set_material(blue)
RightFoot:translate(0.0, -0.2, 0.0)
RightFoot:scale(0.3, 0.2, 0.4)


-- Return our root
return rootnode


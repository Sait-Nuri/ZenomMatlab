function blkStruct = slblocks
% Information for "Blocksets and Toolboxes" subsystem
blkStruct.Name = sprintf('RTXeno-Lab Library\n for target');
blkStruct.OpenFcn = 'RTXenolib';
blkStruct.MaskDisplay = 'disp(''RTXeno-Lab Target'')';

% Information for Simulink Library Browser
Browser(1).Library = 'RTXenolib';
Browser(1).Name    = 'RTXeno-Lab Devices';
Browser(1).IsFlat  = 0;% Is this library "flat" (i.e. no subsystems)?

blkStruct.Browser = Browser;

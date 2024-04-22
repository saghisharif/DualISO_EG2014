function calibrateDualISO(ISO, maxframes)
% Reads the calibration data and creates a .mat file for using in the dualISO program
% Convert CR2 RAW images to tiff with the following command: dcraw -v -4 -D -T *.CR2
% Calibrates based on the ISO 100-800 and ISO 100-1600.
%
%input:
%ISO : ISO setting
%maxframes : maximum number of frames to read.
% Linkoping University

saturationpoint = 15282;

%% Load images
B_100 = getFrameStatsStruct('data/b_100','tiff',maxframes);
W_100 = getFrameStatsStruct('data/w_100','tiff',maxframes);

if(ISO == 800)
 	B_h = getFrameStatsStruct('data/b_800','tiff',maxframes);
 	B_d = getFrameStatsStruct('data/b_dualiso_800','tiff',maxframes);
	W_h = getFrameStatsStruct('data/w_800','tiff',maxframes);
	W_d = getFrameStatsStruct('data/w_dualiso_800','tiff',maxframes);s

else
	B_d = getFrameStatsStruct('data/b_dualiso','tiff',maxframes);
	B_h = getFrameStatsStruct('data/b_1600','tiff',maxframes);
	W_h = getFrameStatsStruct('data/w_1600','tiff',maxframes);
	W_d = getFrameStatsStruct('data/w_dualiso_1600','tiff',maxframes);

end

%% Shading correction
W_100_s = max((double(W_100.img  - B_100.img )) ./ double(saturationpoint - B_100.img ), 0);
W_h_s = max((double(W_h.img  - B_h.img )) ./ double(saturationpoint - B_h.img ), 0);
W_d_s = max(double(W_d.img  - B_d.img ) ./ double(saturationpoint - B_d.img ), 0);

%% Compute scaling

gain_s = W_100_s ./ W_d_s;
gain_s(isnan(gain_s(:))) = 1;
gain_s(isinf(gain_s(:))) = 1;


%% Compute gain

g_100 = (I_100.std.^2 - B_100.std.^2)./(I_100.img - B_100.img);
g_h = (W_h.std.^2 - B_h.std.^2)./(W_h.img - B_h.img);
gain_dualiso = (W_d.std.^2 - B_d.std.^2)./(W_d.img - B_d.img);

blackpoint = B_d.img;
sr = B_d.std;

%% Save the calibration
save(sprintf('calibration_MarkIII_%d.mat',ISO),'gain_s','gain_dualiso','sr','blackpoint');


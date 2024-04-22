function [ res ] = getFrameStatsStruct( fdir, filetype, maxframes )
%Reads raw frames in a directory and computes mean and std.dev. per pixel 
%over a set of frames. To save memory the frames are read one at a time.
%Sort frames by ISO.
%
%input:
%fdir : path to directory of files
%filetype : raw frame type 
%maxframes : maximum number of frames to read.
%
%returns:
%u_b : (per pixel) estiamted mean of bias frame
%s_b : (per pixel) estimated std. dev. of bias frame
% Linkoping University


if( nargin < 3)
    maxframes = inf;
end


wildcard = strcat('/*.', filetype);
fpath = strcat(fdir, wildcard);
fprintf('listing %s \n', fpath);
fns = dir(fpath)
if(isempty(fns))
    error('Specified directory is empty');
end

%Read first frame to get width and height
fn = strcat(fdir, '/', fns(1).name);

fm = imread(fn);
fm = single( fm(3:end,3:end));
h = size(fm,1);
w = size(fm,2);
clear fm;

fprintf('Reading frames \n', fpath);


%Read frames & compute running mean
nt = min(length(fns), maxframes)

res = struct;
res.img = zeros(h,w);
res.std = zeros(h,w);

images = struct;

for i=1:nt
  fn = strcat(fdir, '/', fns(i).name);
  fprintf('loading %s \n', fn);
 
    fm = imread(fn);
    fm = single( fm(3:end,3:end));
    fprintf('read %s succsesfully \n', fn);
    images(i).img = fm;
    res.img = res.img+fm;
 
end

res.img = res.img/nt;


fprintf('Computing std dev \n');

for i=1:nt
 res.std =  res.std + (images(i).img - res.img).^2;
 
end

res.std = sqrt(res.std/(nt-1));

clear images fm fn
clear h w

end

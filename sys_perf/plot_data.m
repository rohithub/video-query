clear;
close all;
clc;

num_frame_skips = 6; %[5 10 20 50 100 200]
num_data_points = 1160; %assuming for frame_skip = 5

path = 'C:\Users\Rohit\Documents\GitHub\video-query\sys_perf\logs\log_no_imshow_cache_clr.txt';

file_p = fopen(path,'r');
myContent = textscan(file_p, '%s');
data_matrix = zeros(num_frame_skips, num_data_points);

i = 0;
m = 0;
count = 0;
mode = 0;

while(i < length(myContent{1,1})) 
   i = i+1;
   if(mode == 0)
       if(strcmp(myContent{1,1}(i),'rate'))
           mode = 1; % data collection phase
           i = i+1; % shift one more as the next value is the frame rate value
           m = m + 1;
           count = 1; %reset the index of the array to write data
       else 
           continue;
       end
   else
       if(strcmp(myContent{1,1}(i),'Elapsed'))
           mode = 0; % scan phase
           display(count);
       else
           data_matrix(m,count) = str2double(myContent{1,1}(i));
           count = count + 1;
       end
   end
end
fclose(file_p);

x_axis = zeros(1,num_data_points);
for j=1:num_data_points;
   x_axis(1,i) = 1;
end

figure;
for j=1:size(data_matrix,1)
    subplot(3,2,j);
    plot(data_matrix(j,2:end));
    xlabel('Frame numbers');
    ylabel('Total pixel sum/max sum');
    myStr = sprintf('Frame Skip rate = %d',data_matrix(j,1));
    title(myStr);
end

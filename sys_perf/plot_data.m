clear; close all; clc;

%Log file format (all values in milliseconds)
%Total time, Video Load, Open Chk, IM Grab, IM Retrieve, RGB 2 GRAY, Resize, 
%62434      240         0           60      11          9           1

%Individual Img Proc log file format  (all values in microseconds)
%Grab Time, Retrieve time, Color, Resize

num_data_per_iter = 4; % Tells the number of data points collected for each iteration
num_frame_skips = 0; %[5 10 20 50 100 200]
num_of_iter = 5;
% path = 'C:\Users\Rohit\Documents\Umich Courses\Prof Wenisch Project\Logs\mod_log_no_imshow_all.txt';
path = 'C:\Users\Rohit\Documents\Umich Courses\Prof Wenisch Project\Logs\mod_log_no_imshow.txt';
%path = 'C:\Users\Rohit\Documents\GitHub\video-query\sys_perf\logs\log_no_imshow.txt';

%Read the log file, scan to get each data point and close the file
file_p = fopen(path,'r');
myContent = textscan(file_p, '%s');
fclose(file_p);

%Total number of iterations is equal to total data points/data points per
%iteration
tot_iters = (length(myContent{1,1}))/num_data_per_iter;
data_matrix = zeros(num_data_per_iter, ceil(tot_iters/num_of_iter));
sum_matrix = zeros(num_data_per_iter, ceil(tot_iters/num_of_iter));
count = 1;
for i=1:tot_iters
    for j=1:num_data_per_iter
        data_matrix(j,mod(i, 6959)+1) = str2double(myContent{1,1}(count));
        count = count + 1;
    end
    
    if(mod(i, 6959) == 0)
    sum_matrix = sum_matrix + data_matrix;
    end
end


sum_matrix = sum_matrix./num_of_iter;
% plot_titles = {'Total time', 'Video Load', 'Check Opened', 'IM Grab', 'IM Retrieve', 'RGB2Gray', 'Resize'};
plot_titles = {'IM Grab', 'IM Retrieve', 'RGB2Gray', 'Resize'};
figure;

x = 1:(size(data_matrix,2)-1);

% plot(x, data_matrix(1, 2:end), 'r+--', x, data_matrix(2, 2:end), 'go--', x, data_matrix(3, 2:end), 'bx--', x, data_matrix(4, 2:end), 'k*--');
% xlabel('Frame Number');
% ylabel('Elapsed time (usec)');
% title('Time taken for processing each frame of the input video');
% legend('Grab time', 'Retrieve time', 'RGB to Gray time', 'Resize time 224x224');
% hold on;
for j=1:size(sum_matrix,1)
    
%     plot(sum_matrix(j,2:end),'+--');
%     xlabel('Iteration #');
%     ylabel('Elapsed time (uSec)');
    
    subplot(2,2,j);
    histogram(sum_matrix(j,1:end));
    
    if(j==1)
        xlabel('Time for 1 operation (msec)');
    else
        xlabel('Time for 1 operation (usec)');
    end
    ylabel('Number of frames');
    title(plot_titles{1,j});
    
    grid on;

end
%legend('Grab time', 'Retrieve time', 'RGB2Gray', 'Resize');
% hold off;
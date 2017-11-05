clear;
close all;
clc;

%Log file format (all values in milliseconds)
%Total time, Video Load, Open Chk, IM Read, RGB 2 GRAY, Resize, 
%66913       275         0         79       15          3

num_data_per_iter = 6; % Teel the number of data points collected for each iteration
num_frame_skips = 0; %[5 10 20 50 100 200]
%path = 'C:\Users\Rohit\Documents\GitHub\video-query\sys_perf\logs\log_no_imshow_cache_clr.txt';
path = 'C:\Users\Rohit\Documents\GitHub\video-query\sys_perf\logs\log_no_imshow.txt';

%Read the log file, scan to get each data point and close the file
file_p = fopen(path,'r');
myContent = textscan(file_p, '%s');
fclose(file_p);

%Total number of iterations is equal to total data points/data points per
%iteration
tot_iters = length(myContent{1,1})/num_data_per_iter;
data_matrix = zeros(num_data_per_iter, tot_iters);

count = 1;
for i=1:tot_iters
    for j=1:num_data_per_iter
        data_matrix(j,i) = str2double(myContent{1,1}(count));
        count = count + 1;
    end
end

plot_titles = {'Total time', 'Video Load', 'Check Opened', 'IM Read', 'RGB2Gray', 'Resize'};
figure;
for j=1:size(data_matrix,1)
    subplot(3,2,j);
    plot(data_matrix(j,2:end));
    xlabel('Iteration #');
    ylabel('Elapsed time');
    title(plot_titles{1,j});
end

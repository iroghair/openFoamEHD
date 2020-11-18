YRES = [8 16 32 64 128 256 512 1024 2048];
data = cell(size(YRES));
V_bot = 1;
l2 = zeros(1,length(YRES));
for i = 1:length(YRES)
    % sample_1_(ncells) is at 1E-7 seconds
    % sample_2_(ncells) is at 2E-7 seconds
    % sample_3_(ncells) is at 3E-7 seconds
    fname = ['sample_3_' num2str(YRES(i)) '.log'];
    data{i} = importdata(fname);
    % Phase 1 is the conductive phase
    exact = data{i}(:,3)*V_bot + (1-data{i}(:,3)).*(1-2.*data{i}(:,1))
    x = abs(exact-data{i}(:,2));
    l2(i) = norm(x,2);
    linf(i) = norm(x,inf);
end
figure;
% Do 2*YRES because the mesh is made of 2 blocks of YRES cells each
loglog(abs(max(data{i}(:,1)-min(data{i}(:,1))))./(2*YRES),l2,'-x',abs(max(data{i}(:,1)-min(data{i}(:,1))))./(2*YRES),linf,'-o');
xlabel('Mesh size [m]');
ylabel('L_2_,_\infty norm of error vector [-]');
legend({'L_2 norm','L_\infty norm'},'Position',[0.75,0.2,0.1,0])
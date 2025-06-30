#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>

GtkWidget *entry_voltage;
GtkWidget *entry_power;
GtkWidget *label_result;

GtkWidget *entry_power_duty;
GtkWidget *entry_power_max;
GtkWidget *label_result_duty;

GtkWidget *create_battery_page();
GtkWidget *create_other_page();
void calculate_duty(GtkWidget *widget, gpointer data);
void on_clear_battery_clicked(GtkWidget *widget, gpointer data);
void on_clear_duty_clicked(GtkWidget *widget, gpointer data);

void on_calculate_clicked(GtkWidget *widget, gpointer data)
{
	const char *voltage_str = gtk_entry_get_text(GTK_ENTRY(entry_voltage));
	const char *power_str = gtk_entry_get_text(GTK_ENTRY(entry_power));

	float voltage = atof(voltage_str);
	float power = atof(power_str);

	if (voltage <= 0 || power <= 0)
	{
		gtk_label_set_text(GTK_LABEL(label_result), "请输入有效的电压和功率！");
		return;
	}

	float battery_number = voltage / 3.6;
	float OVERVOLTAGE_FAULT = battery_number * 4.5;
	float OVERVOLTAGE_RECOVER = battery_number * 4.0;
	float current = power / (battery_number * 3.2);
	float UNDERVOLTAGE_FAULT = battery_number * 2.0;
	float UNDERVOLTAGE_RECOVER = battery_number * 2.5;
	float MAXVOLTAGE = battery_number * 3.2;

	char result[512];
	snprintf(result, sizeof(result),
			 "电池数量: %.1f\n"
			 "过压保护: %.1f V\n"
			 "过压恢复: %.1f V\n"
			 "欠压保护: %.1f V\n"
			 "欠压恢复: %.1f V\n"
			 "母线电流: %.2f A\n"
			 "最大电压: %.2f V",
			 battery_number, OVERVOLTAGE_FAULT, OVERVOLTAGE_RECOVER,
			 UNDERVOLTAGE_FAULT, UNDERVOLTAGE_RECOVER, current, MAXVOLTAGE);

	gtk_label_set_text(GTK_LABEL(label_result), result);
}

int main(int argc, char *argv[])
{
	printf("程序启动\n");
	fflush(stdout);

	gtk_init(&argc, &argv);

	GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "电池参数计算器");
	gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	// // 设置窗口始终在最前面
	// gtk_window_set_keep_above(GTK_WINDOW(window), TRUE);
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	// 创建一个 notebook（多标签页容器），可以用于放多个页面
	GtkWidget *notebook = gtk_notebook_new();

	// 调用你自己封装的函数，创建电池计算的页面（返回的是一个 GtkGrid）
	GtkWidget *battery_page = create_battery_page();

	// 创建第二个页面的内容，这里暂时只放一个简单的文本标签
	GtkWidget *other_page = create_other_page(); // gtk_label_new("这里是第二页，待实现");

	// 把第一个页面加入 notebook，并给这个 tab 命名为“电池计算”
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), battery_page, gtk_label_new("电池计算"));

	// 把第二个页面加入 notebook，并命名为“其他功能”
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), other_page, gtk_label_new("占空比计算"));

	// 将 notebook 添加到主窗口中，这样 notebook 中的页面就会显示出来
	gtk_container_add(GTK_CONTAINER(window), notebook);

	gtk_widget_show_all(window);
	gtk_main();

	return 0;
}

GtkWidget *create_battery_page()
{
	GtkWidget *grid = gtk_grid_new();
	gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);
	gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
	gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
	gtk_container_set_border_width(GTK_CONTAINER(grid), 15);

	GtkWidget *label1 = gtk_label_new("额定电压 (V):");
	entry_voltage = gtk_entry_new();

	GtkWidget *label2 = gtk_label_new("额定功率 (W):");
	entry_power = gtk_entry_new();

	GtkWidget *btn_calculate = gtk_button_new_with_label("计算");
	GtkWidget *btn_clear = gtk_button_new_with_label("清除");
	g_signal_connect(btn_clear, "clicked", G_CALLBACK(on_clear_battery_clicked), NULL);

	//回车键可以计算结果
	g_signal_connect(entry_voltage, "activate", G_CALLBACK(on_calculate_clicked), NULL);
	g_signal_connect(entry_power, "activate", G_CALLBACK(on_calculate_clicked), NULL);

	g_signal_connect(btn_calculate, "clicked", G_CALLBACK(on_calculate_clicked), NULL);

	label_result = gtk_label_new("请输入数据并点击计算");

	gtk_grid_attach(GTK_GRID(grid), label1, 0, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), entry_voltage, 1, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), label2, 0, 1, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), entry_power, 1, 1, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), btn_calculate, 0, 2, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), btn_clear, 1, 2, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), label_result, 0, 3, 2, 1);

	return grid; // 🔁 返回这个页面
}

GtkWidget *create_other_page()
{
	// GtkWidget *label = gtk_label_new("这里是第二页的内容");
	// GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
	// gtk_box_pack_start(GTK_BOX(box), label, TRUE, TRUE, 0);

	GtkWidget *grid = gtk_grid_new();
	gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);
	gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
	gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
	gtk_container_set_border_width(GTK_CONTAINER(grid), 15);

	GtkWidget *label1 = gtk_label_new("目标功率 (V):");
	entry_power_duty = gtk_entry_new();
	GtkWidget *label2 = gtk_label_new("最大功率(A):");
	entry_power_max = gtk_entry_new();

	GtkWidget *btn_calculate = gtk_button_new_with_label("计算");
	GtkWidget *btn_clear = gtk_button_new_with_label("清除");
	g_signal_connect(btn_clear, "clicked", G_CALLBACK(on_clear_duty_clicked), NULL);

	// 回车键可以计算结果
	g_signal_connect(entry_power_duty, "activate", G_CALLBACK(calculate_duty), NULL);
	g_signal_connect(entry_power_max, "activate", G_CALLBACK(calculate_duty), NULL);

	g_signal_connect(btn_calculate, "clicked", G_CALLBACK(calculate_duty), NULL);
	label_result_duty = gtk_label_new("请输入数据并点击计算");

	gtk_grid_attach(GTK_GRID(grid), label1, 0, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), entry_power_duty, 1, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), label2, 0, 1, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), entry_power_max, 1, 1, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), btn_calculate, 0, 2, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), btn_clear, 1, 2, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), label_result_duty, 0, 3, 2, 1);

	return grid;
}

// 占空比计算

void calculate_duty(GtkWidget *widget, gpointer data)
{
	const char *power_str = gtk_entry_get_text(GTK_ENTRY(entry_power_duty));
	const char *powerMax_str = gtk_entry_get_text(GTK_ENTRY(entry_power_max));

	double power = atof(power_str);
	double powerMax = atof(powerMax_str);
	if (power <= 25)
	{
		gtk_label_set_text(GTK_LABEL(label_result_duty), "目标功率必须大于25");
		return;
	}

	if (powerMax <= 25)
	{
		gtk_label_set_text(GTK_LABEL(label_result_duty), "最大功率必须大于25");
		return;
	}

	int duty = (((power - 25) * 80) / (powerMax - 25) + 10);
	 duty = duty > 100 ? 100 : duty;

	char resultDuty[256];
	// sprintf(resultDuty, "占空比为：%d", duty);
	snprintf(resultDuty, sizeof(resultDuty), "占空比为：%d%%", duty);
	gtk_label_set_text(GTK_LABEL(label_result_duty), resultDuty);
}

// 清除电池计算输入
void on_clear_battery_clicked(GtkWidget *widget, gpointer data)
{
	gtk_entry_set_text(GTK_ENTRY(entry_voltage), "");
	gtk_entry_set_text(GTK_ENTRY(entry_power), "");
	gtk_label_set_text(GTK_LABEL(label_result), "请输入数据并点击计算");
}

// 清除占空比计算输入
void on_clear_duty_clicked(GtkWidget *widget, gpointer data)
{
	gtk_entry_set_text(GTK_ENTRY(entry_power_duty), "");
	gtk_entry_set_text(GTK_ENTRY(entry_power_max), "");
	gtk_label_set_text(GTK_LABEL(label_result_duty), "请输入数据并点击计算");
}

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>

GtkWidget *entry_voltage;
GtkWidget *entry_power;
GtkWidget *label_result;

void on_calculate_clicked(GtkWidget *widget, gpointer data) {
	const char *voltage_str = gtk_entry_get_text(GTK_ENTRY(entry_voltage));
	const char *power_str = gtk_entry_get_text(GTK_ENTRY(entry_power));
	
	float voltage = atof(voltage_str);
	float power = atof(power_str);
	
	if (voltage <= 0 || power <= 0) {
		gtk_label_set_text(GTK_LABEL(label_result), "请输入有效的电压和功率！");
		return;
	}
	
	float battery_number = voltage / 3.6;
	float OVERVOLTAGE_FAULT = battery_number * 4.5;
	float OVERVOLTAGE_RECOVER = battery_number * 4.0;
	float current = power / (battery_number * 3.2);
	float UNDERVOLTAGE_FAULT = battery_number * 2.0;
	float UNDERVOLTAGE_RECOVER = battery_number * 2.5;
	
	char result[512];
	snprintf(result, sizeof(result),
		"电池数量: %.1f\n"
		"过压保护: %.1f V\n"
		"过压恢复: %.1f V\n"
		"欠压保护: %.1f V\n"
		"欠压恢复: %.1f V\n"
		"母线电流: %.2f A",
		battery_number, OVERVOLTAGE_FAULT, OVERVOLTAGE_RECOVER,
		UNDERVOLTAGE_FAULT, UNDERVOLTAGE_RECOVER, current);
	
	gtk_label_set_text(GTK_LABEL(label_result), result);
}

int main(int argc, char *argv[]) {
	printf("程序启动\n");
	fflush(stdout);
	
	gtk_init(&argc, &argv);
	
	GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "电池参数计算器");
	gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	
	GtkWidget *grid = gtk_grid_new();
	gtk_container_add(GTK_CONTAINER(window), grid);
	gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
	gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
	gtk_container_set_border_width(GTK_CONTAINER(grid), 15);
	
	GtkWidget *label1 = gtk_label_new("额定电压 (V):");
	entry_voltage = gtk_entry_new();
	
	GtkWidget *label2 = gtk_label_new("额定功率 (W):");
	entry_power = gtk_entry_new();
	
	GtkWidget *btn_calculate = gtk_button_new_with_label("计算");
	g_signal_connect(btn_calculate, "clicked", G_CALLBACK(on_calculate_clicked), NULL);
	
	label_result = gtk_label_new("请输入数据并点击计算");
	
	gtk_grid_attach(GTK_GRID(grid), label1, 0, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), entry_voltage, 1, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), label2, 0, 1, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), entry_power, 1, 1, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), btn_calculate, 0, 2, 2, 1);
	gtk_grid_attach(GTK_GRID(grid), label_result, 0, 3, 2, 1);
	
	gtk_widget_show_all(window);
	gtk_main();
	
	return 0;
}


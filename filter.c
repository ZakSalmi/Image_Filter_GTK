#include <gtk/gtk.h>
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include "helpers.h"

//functions
void optionMenu();
void value_changed(GtkRange *range, gpointer win);
void checkRadioBtn();
char fileopener(GtkWidget* button, gpointer window);
void filtering();
void submit();
void gtkStart();

//globals
gchar *filename;
gint scaleValue;
GtkWidget *mainWindow;
gint selectedRadio;
GtkWidget *filter1, *filter2, *filter3, *filter4, *filter5, *filter6, *filter7, *filter8;

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);
    optionMenu();
    
    return 0;
}

void filtering()
{
    char *infile = filename;
    char *outfile = "out.bmp";
    
    FILE *inptr = fopen(infile, "r");

    
    FILE *outptr = fopen(outfile, "w");


    // Read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // Read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    int height = abs(bi.biHeight);
    int width = bi.biWidth;

    // Allocate memory for image
    RGBTRIPLE(*localimage)[width] = calloc(height, width * sizeof(RGBTRIPLE));

    // Determine padding for scanlines
    int padding = (4 - (width * sizeof(RGBTRIPLE)) % 4) % 4;

    // Iterate over infile's scanlines
    for (int i = 0; i < height; i++)
    {
        // Read row into pixel array
        fread(localimage[i], sizeof(RGBTRIPLE), width, inptr);

        // Skip over padding
        fseek(inptr, padding, SEEK_CUR);
    }

    switch (selectedRadio)
    {
        // Blur
        case 3:
            blur(height, width, localimage);
            break;

        // Edges
        case 5:
            edges(height, width, localimage);
            break;

        // Grayscale
        case 1:
            grayscale(height, width, localimage);
            break;

        // Reflect
        case 8:
            reflect(height, width, localimage);
            break;
         // Reflect
        case 4:
            sepia(height, width, localimage);
            break;

        case 2:
            black_white(height, width, localimage);

        case 7:
            adjsut_luminosity( height,  width, localimage, scaleValue);
            break;

        case 6:
            adjsut_contrast( height,  width, localimage,scaleValue);
            break;
    }
    // Write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // Write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // Write new pixels to outfile
    for (int i = 0; i < height; i++)
    {
        // Write row to outfile
        fwrite(localimage[i], sizeof(RGBTRIPLE), width, outptr);

        // Write padding at end of row
        for (int k = 0; k < padding; k++)
        {
            fputc(0x00, outptr);
        }
    }

    // Free memory for image
    free(localimage);

    // Close infile
    fclose(inptr);

    // Close outfile
    fclose(outptr);
}

void optionMenu()
{
    GtkWidget *new_window, *okBtn, *closeBtn, *upBtn;
    GtkWidget *vbox, *hbox, *halign, *valign, *savelabel, *saveBtn;
    GtkWidget *label, *label2, *optionsgrid, *hscale, *label3, *label4;

    /* Create new window */    
    new_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(new_window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(new_window), 400, 400);
    gtk_container_set_border_width(GTK_CONTAINER(new_window), 20);
    gtk_window_set_title(GTK_WINDOW(new_window), "Importer");

    /* filter radios */
    
    filter1 = gtk_radio_button_new_with_label (NULL, "GreyScale");
    filter2 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (filter1), "Black&White");
    filter3 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (filter1), "Blur");
    filter4 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (filter1), "Sepia");
    filter5 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (filter1), "Edge Detection");
    filter6 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (filter1), "contrast");
    filter7 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (filter1), "Luminosity");
    filter8 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (filter1), "Reflect");

    /* create the grid*/
    optionsgrid = gtk_grid_new ();
    gtk_grid_set_row_spacing(optionsgrid, 10);
    gtk_grid_set_column_spacing(optionsgrid, 50);
    gtk_container_add (GTK_CONTAINER(new_window), optionsgrid);
    
    label = gtk_label_new ("Téléchargez l'image:");
    upBtn = gtk_button_new_with_label ("Télécharger");

    gtk_grid_attach (GTK_GRID (optionsgrid), label, 0, 0, 1, 1);
    gtk_grid_attach_next_to (GTK_GRID (optionsgrid), upBtn, label, 1, 1, 1);


    label2 = gtk_label_new ("Select one filter :");
    
    gtk_grid_attach (GTK_GRID (optionsgrid), label2, 0, 1, 1, 1);
    gtk_grid_attach (GTK_GRID (optionsgrid), filter1, 0, 2, 1, 1);
    gtk_grid_attach (GTK_GRID (optionsgrid), filter2, 0, 3, 1, 1);
    gtk_grid_attach (GTK_GRID (optionsgrid), filter3, 0, 4, 1, 1);
    gtk_grid_attach (GTK_GRID (optionsgrid), filter4, 0, 5, 1, 1);
    gtk_grid_attach (GTK_GRID (optionsgrid), filter5, 1, 2, 1, 1);
    gtk_grid_attach (GTK_GRID (optionsgrid), filter6, 1, 3, 1, 1);
    gtk_grid_attach (GTK_GRID (optionsgrid), filter7, 1, 4, 1, 1);
    gtk_grid_attach (GTK_GRID (optionsgrid), filter8, 1, 5, 1, 1);

    g_signal_connect(G_OBJECT(upBtn), "clicked", G_CALLBACK(fileopener), new_window);


    /* Scale */
    label4 = gtk_label_new ("Veuillez choisir l'intensité du filtre:");
    hscale = gtk_hscale_new_with_range(0, 100, 1);
    gtk_scale_set_draw_value(GTK_SCALE(hscale), FALSE);
    gtk_widget_set_size_request(hscale, 150, -1);
    label3 = gtk_label_new("0");
    gtk_misc_set_alignment(GTK_MISC(label3), 0, 1);
    
    gtk_grid_attach (GTK_GRID (optionsgrid), label4, 0, 6, 1, 1);
    gtk_grid_attach (GTK_GRID (optionsgrid), hscale, 0, 7, 1, 1);
    gtk_grid_attach (GTK_GRID (optionsgrid), label3, 1, 7, 1, 1);

    g_signal_connect(hscale, "value-changed",G_CALLBACK(value_changed), label3);
    
    /* create buttons */
    
    okBtn = gtk_button_new_with_label("Ok");
    gtk_widget_set_size_request(okBtn, 70, 30);
    gtk_grid_attach (GTK_GRID (optionsgrid), okBtn, 1, 9, 1, 1);

    closeBtn = gtk_button_new_with_label("Fermer");
    gtk_widget_set_size_request(closeBtn, 70, 30);
    gtk_grid_attach (GTK_GRID (optionsgrid), closeBtn, 0, 9, 1, 1);

    g_signal_connect(G_OBJECT(closeBtn), "clicked",G_CALLBACK(gtk_window_close), G_OBJECT(new_window));
    
    g_signal_connect(G_OBJECT(okBtn), "clicked",G_CALLBACK(submit), new_window);
    
    gtk_widget_show_all(new_window);
    gtk_main();
}

void submit(GtkWidget* button, gpointer window)
{
    checkRadioBtn();
    filtering();
    gtk_widget_destroy(window);
    gtkStart();
}

void value_changed(GtkRange *range, gpointer win) {
    
   scaleValue = gtk_range_get_value(range);
   gchar *str = g_strdup_printf("%d", scaleValue);    
   gtk_label_set_text(GTK_LABEL(win), str);
   g_free(str);
}

char fileopener(GtkWidget* button, gpointer window)
{
    GtkWidget *dialog;
    dialog = gtk_file_chooser_dialog_new("Choisissez un fichier -BMP UNIQUEMENT-",
    					  GTK_WINDOW(window), 
                                        GTK_FILE_CHOOSER_ACTION_OPEN, GTK_STOCK_OK,
                                        GTK_RESPONSE_OK,GTK_STOCK_CANCEL,
                                        GTK_RESPONSE_CANCEL, NULL);
    
    gtk_widget_show_all(dialog);
    
    GtkFileFilter *filter = gtk_file_filter_new();
	gtk_file_filter_add_pattern(filter,"*.bmp");
    gtk_file_chooser_add_filter( dialog, filter );
    
    gint resp = gtk_dialog_run(GTK_DIALOG(dialog));
    if (resp == GTK_RESPONSE_OK)
    {
        filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        g_print("%s\n", filename);
    }
    else
        g_print("Annuler\n");
    gtk_widget_destroy(dialog);
}

void checkRadioBtn()
{
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON (filter1)) == TRUE)
    {
        selectedRadio = 1;
    }else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON (filter2)) == TRUE)
    {
        selectedRadio = 2;
    }else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON (filter3)) == TRUE)
    {
        selectedRadio = 3;
    }else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON (filter4)) == TRUE)
    {
        selectedRadio = 4;
    }else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON (filter5)) == TRUE)
    {
        selectedRadio = 5;
    }else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON (filter6)) == TRUE)
    {
        selectedRadio = 6;
    }else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON (filter7)) == TRUE)
    {
        selectedRadio = 7;
    }else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON (filter8)) == TRUE)
    {
        selectedRadio = 8;
    }
}

void gtkStart()
{
    GtkWidget *vbox;
    GtkWidget *menubar;
    GtkWidget *fileMenu;
    GtkWidget *fileMi;
    GtkWidget *saveMi;
    GtkWidget *openMi;
    GtkWidget *quitMi;
    GtkWidget *image, *grid, *edited;
    GtkWidget *sep;
    
    mainWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(mainWindow), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(mainWindow), 600, 600);
    gtk_window_set_title(GTK_WINDOW(mainWindow), "ImageFilter");

    GtkAccelGroup *accel_group = NULL;

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(mainWindow),grid);
    gtk_grid_set_row_spacing(grid, 10);
    gtk_grid_set_column_spacing(grid, 300);

    menubar = gtk_menu_bar_new();
    fileMenu = gtk_menu_new();

    accel_group = gtk_accel_group_new();
    gtk_window_add_accel_group(GTK_WINDOW(mainWindow), accel_group);

    fileMi = gtk_menu_item_new_with_mnemonic("_File");
    openMi = gtk_image_menu_item_new_from_stock(GTK_STOCK_OPEN, NULL);
    saveMi = gtk_image_menu_item_new_from_stock(GTK_STOCK_SAVE_AS, NULL);
    sep = gtk_separator_menu_item_new();
    quitMi = gtk_image_menu_item_new_from_stock(GTK_STOCK_QUIT, accel_group);

    gtk_menu_item_set_submenu(GTK_MENU_ITEM(fileMi), fileMenu);
    gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), openMi);
    gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), saveMi);
    gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), sep);
    gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), quitMi);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), fileMi);
    gtk_grid_attach(GTK_GRID(grid), menubar,0,0,2,1);

    GtkWidget *label1, *label2;
    label1 = gtk_label_new ("Image originale");
    label2 = gtk_label_new ("Image éditée");
  
    gtk_grid_attach(GTK_GRID(grid), label1, 0, 1, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), label2, 1, 1, 2, 1);


    image = gtk_image_new_from_file(filename);
    gtk_grid_attach(GTK_GRID(grid), image, 0, 2, 2, 1);
    
    edited = gtk_image_new_from_file("out.bmp");
    gtk_grid_attach(GTK_GRID(grid), edited, 1, 2, 2, 1);
    
    g_signal_connect(G_OBJECT(mainWindow), "destroy",
          G_CALLBACK(gtk_main_quit), NULL);

    g_signal_connect(G_OBJECT(quitMi), "activate",
          G_CALLBACK(gtk_main_quit), NULL);
  
    g_signal_connect(G_OBJECT(openMi), "activate",
          G_CALLBACK(optionMenu), NULL);

    gtk_widget_show_all(mainWindow);

}

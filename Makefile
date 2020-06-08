# Clang options
CC = clang

#CFLAGS = -Ofast -std=c99 -Wall -pedantic
CFLAGS = -g -Og -std=c99 -Wall -pedantic

LDFLAGS = -lm


CFLAGS += -I/usr/local/include/cairo
LDFLAGS += -L/usr/local/lib/cairo -lcairo

SOURCE = points.c plot_color.c plot_line.c \
		plot_area.c plot_title.c plot_axis.c \
		plot_data.c colortables.c plot_palette.c plot_coloraxis.c \
		plot_series.c plot_colormap.c plot_polygon.c plot_contour.c \
		plot.c \
		main.c

TARGET = plot

OBJ = $(SOURCE:.c=.o)

all : $(SOURCE) $(TARGET)

$(TARGET) : $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) -o $@

.c.o:
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	@touch $(TARGET) $(OBJ)
	rm -f $(TARGET) $(OBJ)

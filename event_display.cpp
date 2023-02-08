#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <raylib.h>
#include "my_camera.h"

#include <TCanvas.h>
#include <TGraph.h>
#include <TFile.h>
#include <TTree.h>
#define MAX(a,b) (a>b ? a : b)
#define MIN(a,b) (a<b ? a : b)

#define SUMMARY_BAR_SAMPLES 100


int daq_channel_module[] = { 0, 0, 0, 0, 0, 4, 4, 4, 2, 2, 0, 0, 4, 4, 4, 0, 0, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 3, 5, 5, 4, 4, 1, 2, 2, 2, 4, 4, 4, 4, 4, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 5, 5, 5, 5, 1, 3, 3, 3, 0, 0, 0, 0, 4, 0, 2, 2, 2, 0, 0, 2, 2, 4, 4, 2, 2, 2, 5, 5, 5, 5, 5, 5, 3, 3, 3, 3, 3, 5, 5, 5, 5, 3, 3, 3, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7};
int daq_channel_chan_num[] = {14, 13, 12, 11, 10, 3, 4, 5, 15, 14, 9, 8, 15, 14, 13, 6, 7, 0, 1, 2, 3, 4, 5, 6, 13, 12, 11, 0, 0, 1, 6, 7, 7, 10, 9, 8, 12, 11, 10, 9, 8, 15, 14, 13, 12, 11, 10, 9, 1, 2, 3, 4, 2, 3, 4, 5, 8, 5, 6, 7, 0, 1, 2, 3, 0, 4, 0, 1, 2, 5, 15, 3, 4, 1, 2, 5, 6, 7, 15, 14, 13, 12, 11, 10, 15, 14, 13, 12, 11, 6, 7, 9, 8, 10, 9, 8, 0, 15, 1, 14, 2, 13, 3, 12, 4, 11, 5, 10, 15, 0, 14, 1, 13, 2, 12, 3, 11, 4, 10, 5};
static const double pmt_x[] = {170.918, 112.621, 24.147, -70.796, -146.770, -183.417, -170.918, -112.621, -24.148, 70.796, 146.770, 183.417, 170.918, 112.621, 24.147, -70.796, -146.770, -183.417, -170.918, -112.621, -24.148, 70.796, 146.770, 183.417, 170.918, 112.621, 24.147, -70.796, -146.770, -183.417, -170.918, -112.621, -24.148, 70.796, 146.770, 183.417, 170.918, 112.621, 24.147, -70.796, -146.770, -183.417, -170.918, -112.621, -24.148, 70.796, 146.770, 183.417, 170.918, 112.621, 24.147, -70.796, -146.770, -183.417, -170.918, -112.621, -24.148, 70.796, 146.770, 183.417, 55.000, 27.500, -27.500, -55.000, -27.500, 27.500, 106.252, 77.782, 28.470, -28.470, -77.782, -106.252, -106.252, -77.782, -28.470, 28.470, 77.782, 106.252, 55.000, 27.500, -27.500, -55.000, -27.500, 27.500, 106.252, 77.782, 28.470, -28.470, -77.782, -106.252, -106.252, -77.782, -28.470, 28.470, 77.782, 106.252, 195.715, 167.219, 75.543, -27.512, -120.172, -194.731, -195.715, -167.219, -75.543, 27.511, 120.172, 194.731, 195.715, 167.219, 75.543, -27.512, -120.172, -194.731, -195.715, -167.219, -75.543, 27.511, 120.172, 194.731};
static const double pmt_y[] = {-70.796, -146.770, -183.417, -170.918, -112.621, -24.147, 70.796, 146.770, 183.417, 170.918, 112.621, 24.148, -70.796, -146.770, -183.417, -170.918, -112.621, -24.147, 70.796, 146.770, 183.417, 170.918, 112.621, 24.148, -70.796, -146.770, -183.417, -170.918, -112.621, -24.147, 70.796, 146.770, 183.417, 170.918, 112.621, 24.148, -70.796, -146.770, -183.417, -170.918, -112.621, -24.147, 70.796, 146.770, 183.417, 170.918, 112.621, 24.148, -70.796, -146.770, -183.417, -170.918, -112.621, -24.147, 70.796, 146.770, 183.417, 170.918, 112.621, 24.148, -0.000, -47.631, -47.631, -0.000, 47.631, 47.631, -28.470, -77.782, -106.252, -106.252, -77.782, -28.470, 28.470, 77.782, 106.252, 106.252, 77.782, 28.470, -0.000, -47.631, -47.631, -0.000, 47.631, 47.631, -28.470, -77.782, -106.252, -106.252, -77.782, -28.470, 28.470, 77.782, 106.252, 106.252, 77.782, 28.470, -25.766, -128.312, -182.377, -208.972, -156.611, -80.660, 25.766, 128.312, 182.377, 208.972, 156.611, 80.660, -25.766, -128.312, -182.377, -208.972, -156.611, -80.660, 25.766, 128.312, 182.377, 208.972, 156.611, 80.660};
static const double pmt_z[] = {120.000, 120.000, 120.000, 120.000, 120.000, 120.000, 120.000, 120.000, 120.000, 120.000, 120.000, 120.000, 60.000, 60.000, 60.000, 60.000, 60.000, 60.000, 60.000, 60.000, 60.000, 60.000, 60.000, 60.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, -60.000, -60.000, -60.000, -60.000, -60.000, -60.000, -60.000, -60.000, -60.000, -60.000, -60.000, -60.000, -120.000, -120.000, -120.000, -120.000, -120.000, -120.000, -120.000, -120.000, -120.000, -120.000, -120.000, -120.000, 147.000, 147.000, 147.000, 147.000, 147.000, 147.000, 147.000, 147.000, 147.000, 147.000, 147.000, 147.000, 147.000, 147.000, 147.000, 147.000, 147.000, 147.000, -147.000, -147.000, -147.000, -147.000, -147.000, -147.000, -147.000, -147.000, -147.000, -147.000, -147.000, -147.000, -147.000, -147.000, -147.000, -147.000, -147.000, -147.000, 160.600, 147.500, 160.600, 147.500, 160.600, 147.500, 160.600, 147.500, 160.600, 147.500, 160.600, 147.500, -160.600, -147.500, -160.600, -147.500, -160.600, -147.500, -160.600, -147.500, -160.600, -147.500, -160.600, -147.500};
static const int N_PMT = sizeof(pmt_x)/sizeof(pmt_x[0]);
Vector3 PMT_POSITIONS[N_PMT]; // Will be initialized in main()

static float acrylic_z[] = {2083.0, 1241.81, 1217.74, 1199.07, 1175.0, -1175.0, -1199.07, -1218.68, -1243.7};
static float acrylic_r[] = {240.0, 240.0, 1005.91, 1600.0, 1610.0, 1610.0, 1600.0, 795.906, 0.0};
static const int ACRYLIC_LEN = sizeof(acrylic_r)/sizeof(acrylic_r[0]);
static const Color ACRYLIC_COLOR = {255,255,255, 25};


static int get_index(const int chan, const int offset, const int wf_nsample) {
    return wf_nsample*chan + offset;
}

Color float_to_color(float v, int is_veto) {
    float value = floor(255*v);
    value = value > 255 ? 255 : value;
    value = value < 0 ? 0 : value;
    unsigned char t = (unsigned char)value;
    unsigned char t2 = 255-t; // Low when charge is high
    unsigned char t3 = MIN(t+10, 255);
    Color c = {255, t2, t2, t3};
    //Color c = {255, 0, 0, 255};
    if(is_veto) {
        c = {t2, t2, 255, t3};
        //c = {0, 0, 255, 255};
    }
    return c;
}
Color float_to_color2(float v) {
    float value = floor(255*v);
    value = value > 255 ? 255 : value;
    value = value < 0 ? 0 : value;
    unsigned char t = (unsigned char)value;
    unsigned char t2 = 255-t; // Low when charge is high
    Color c = {255, t2, t2, 255};
    return c;
}

int get_daq_channel(int pmt_id) {
    // For a given PMT ID, gets the corresponding DAQ channel
    return daq_channel_module[pmt_id]*16 + daq_channel_chan_num[pmt_id];
}

typedef struct DisplayData{
    float* values;
    float max_value;
    int nsample;
    int start_idx;
    int end_idx;
} DisplayData;

void grab_event(TTree* t, int idx) {
    t->GetEntry(idx);
}


void draw_acrylic_tank() {
    int i;
    for(i=0;i<ACRYLIC_LEN-1;i++) {
        Vector3 pos1 = {0,0, acrylic_z[i]};
        Vector3 pos2 = {0,0, acrylic_z[i+1]};
        float height = 3;
        DrawCylinderEx(pos1, pos2, acrylic_r[i], acrylic_r[i], 15, ACRYLIC_COLOR);
    }
}

void draw_event_display(DisplayData* disp_data, float frac) {
    int i;
    for(i=0; i<N_PMT;i++) {
        int is_veto = i>= 96;
        float v = disp_data->values[i]/disp_data->max_value;
        if(v>0.05) {
            DrawSphereEx(PMT_POSITIONS[i], 1, 5, 5, float_to_color(v, is_veto));
        }
    }
}

int main(int argc, char** argv) {
    const int screenWidth = 1600;
    const int screenHeight = 900;
    int i;
    char buffer[128];
    float box_values[SUMMARY_BAR_SAMPLES];
    memset(box_values, 0, sizeof(float)*SUMMARY_BAR_SAMPLES);

    int which_event = 0;

    if(argc > 1) {
        which_event = strtol(argv[1], NULL, 0);
    }

    //const char* fn = "run22.root";
    //const char* fn = "self_trigger_run0.root";
    //const char* fn = "self_trigger_run1.root";
    //const char* fn = "run80.root";
    //const char* fn = "run10.root";
    const char* fn = "run84.root";
    //const char* fn = "run75.root";
    //const char* fn = "run71.root";
    //const char* fn = "run73.root";
    //TFile*f = new TFile("self_trigger_run1.root");
    //TFile* f = new TFile("run82.root");
    //TFile* f = new TFile("run8.root");
    TFile* f = new TFile(fn);
    if(!f || !f->IsOpen()) {
        printf("Cannot open ROOT file\n");
        return 1;
    }
    TTree* tree = (TTree*)f->Get("tree");
    uint16_t* FADC_Data = (uint16_t*)malloc(1024*1024*32);
    unsigned int* nsample_ptr = new unsigned int();
    unsigned int nsample;
    tree->SetBranchAddress("FADC", FADC_Data);
    tree->SetBranchAddress("nsample", nsample_ptr);

    int found_count = 0;
    int event_index = which_event;
    //tree->GetEntry(event_index);
    //nsample = *nsample_ptr;

    int n_entries = tree->GetEntries();
    double SUMMED_DATA[N_PMT];
    memset(SUMMED_DATA, 0, sizeof(double)*N_PMT);
    for(i=0; i< n_entries; i++) {
        tree->GetEntry(i);
        nsample = *nsample_ptr;
        //if(nsample > 1000) {
        //    continue;
        //}
        int j;
        for(j=0;j<N_PMT;j++) {
            int k;
            for(k=0;k<nsample;k++) {
                float v = FADC_Data[get_index(j, k, nsample)];
                v = v - 8192;
                v = MIN(v, 5); // Don't allow for overshoot!

                SUMMED_DATA[j] += v;
            }
        }
    }
    double max_value = SUMMED_DATA[0];
    for(i=0;i<N_PMT;i++) {
        if(SUMMED_DATA[i] < max_value) {
            max_value = SUMMED_DATA[i];
        }
    }
    // Normalize
    for(i=0;i<N_PMT;i++) {
        SUMMED_DATA[i] /= max_value;
        printf("%i - %0.3f\n", i, SUMMED_DATA[i]);
    }


    //int event_index;
    //i=0;
    //while(1) {
    //    event_index = i;
    //    tree->GetEntry(i++);
    //    nsample = *nsample_ptr;
    //    if(nsample < 900) {
    //        found_count += 1;
    //    }
    //    if(found_count > which_event) {
    //        break;
    //    }
    //}

    float* WF_DATA = (float*)malloc(sizeof(float)*nsample*128);

    for(i=0;i<ACRYLIC_LEN;i++) {
        acrylic_r[i] = acrylic_r[i]/100.0;
        acrylic_z[i] = acrylic_z[i]/100.0;
    }

    InitWindow(screenWidth, screenHeight, "JSNS2 Event Display");

    // Define the camera to look into our 3d world
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 50.0f, 50.0f, 0.0f };  // Camera position
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 0.0f, 1.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera mode type
    SetTargetFPS(60);


    for(i=0; i<N_PMT;i++) {
         PMT_POSITIONS[i].x = (float)pmt_x[i]/10.0;
         PMT_POSITIONS[i].y = (float)pmt_y[i]/10.0;
         PMT_POSITIONS[i].z = (float)pmt_z[i]/10.0;
    }

    //--------------------------------------------------------------------------------------
    const int START_INDEX = 150;
    const int END_INDEX = 220;

    double ABSOLUTE_MAX_ADC_VALUE = 0;
    float * sum_wf = (float*)malloc(sizeof(float)*nsample);
    memset(sum_wf, 0, sizeof(float)*nsample);
    for(i=0; i < 128*nsample; i++) {
        WF_DATA[i] = FADC_Data[i];
        WF_DATA[i] -= 8192;
        if(WF_DATA[i] > 0) {
            WF_DATA[i] = 0;
        }
        int sample = i%nsample;
        if(sample < START_INDEX || sample > END_INDEX) {
            continue;
        }
        if(WF_DATA[i] < ABSOLUTE_MAX_ADC_VALUE) {
            ABSOLUTE_MAX_ADC_VALUE = WF_DATA[i];
        }
        sum_wf[i%nsample] += WF_DATA[i];
    }

    printf("ABS MAX = %f\n", ABSOLUTE_MAX_ADC_VALUE);
    printf("nsample = %i\n", nsample);
    Vector2 graph_line[SUMMARY_BAR_SAMPLES];
    float max_graph = 0;
    for(i=0;i<SUMMARY_BAR_SAMPLES;i++) {
        int start_idx = ((float)i/SUMMARY_BAR_SAMPLES)*(END_INDEX-START_INDEX) + START_INDEX;
        int end_idx = ((float)(i+1)/SUMMARY_BAR_SAMPLES)*(END_INDEX-START_INDEX) + START_INDEX;
        graph_line[i].x = i;
        if(start_idx == end_idx) {
            graph_line[i].y = sum_wf[start_idx];
        }
        else {
            int j;
            graph_line[i].y=0;
            for(int j=start_idx; j<end_idx;j++) {
                graph_line[i].y+=sum_wf[start_idx];
            }
        }
        if(graph_line[i].y < max_graph) {
            max_graph = graph_line[i].y;
        }
    }
    for(i=0;i<SUMMARY_BAR_SAMPLES;i++) {
        graph_line[i].y /= max_graph;
        box_values[i] = graph_line[i].y;

        // Now move the line to the correct position
        graph_line[i].y = graph_line[i].y*20+30;
        graph_line[i].x = 10 + graph_line[i].x*(screenWidth-20)/(float)SUMMARY_BAR_SAMPLES;
    }

    EnableCursor();
    double time;
    int paused = 0;
    double prev_time;
    // Main game loop
    //
    // Main Draw Loop
    while (!WindowShouldClose()) {
        MyUpdateCamera(&camera);
        time = GetTime();
        float delta_t = time-prev_time;
        if(IsKeyPressed(KEY_SPACE)) {
            paused = !paused;
        }
        if(paused) {
            time=prev_time;
        }


        double RESET_TIME = 10.0;
        // TODO could not truncate, and instead interpolate
        float frac = fmod(time, RESET_TIME)/RESET_TIME;
        int sample_idx = START_INDEX + frac * (END_INDEX-START_INDEX);


        BeginDrawing();
            ClearBackground(BLACK);
            BeginMode3D(camera);

                for(i=0; i<N_PMT;i++) {
                    int is_veto = i>= 96;
                     //   void DrawSphereEx(Vector3 centerPos, float radius, int rings, int slices, Color color);            // Draw sphere with extended parameters
                    int daq_channel = get_daq_channel(i);
                    int offset = get_index(daq_channel, sample_idx, nsample);
                    float v = WF_DATA[offset]/ABSOLUTE_MAX_ADC_VALUE;
                    v = SUMMED_DATA[daq_channel];
                    if(v>0.05) {
                        DrawSphereEx(PMT_POSITIONS[i], 1, 5, 5, float_to_color(v, is_veto));
                    }
                    //DrawSphere(pmt_pos, 1, RED);
                }
                // Draw the acrlyic tank
                draw_acrylic_tank();


            EndMode3D();
            DrawRectangle(10, 10, screenWidth-20, 10, GRAY);
            for(int i=0; i <SUMMARY_BAR_SAMPLES; i++) {
                if(i>frac*SUMMARY_BAR_SAMPLES) {
                    break;
                }

                DrawRectangle((screenWidth-20 )* i/(float)SUMMARY_BAR_SAMPLES, 10, (screenWidth-20 )* 1.03/(float)SUMMARY_BAR_SAMPLES, 10, float_to_color2(box_values[i]));
            }
            DrawLineStrip(graph_line, SUMMARY_BAR_SAMPLES, (Color){255,0,0,255});

            snprintf(buffer, 128, "Event ID = %i", event_index);
            DrawText(fn, 10, 40, 20, RAYWHITE);
            DrawText(buffer, 10, 70, 20, RAYWHITE);
            snprintf(buffer, 128, "T=%dns", sample_idx*2);
            DrawText(buffer, 10, 100, 20, RAYWHITE);
        EndDrawing();
        //----------------------------------------------------------------------------------
        prev_time = time;
    }
    CloseWindow();
    return 0;
}

#pragma once

#include "ResultsStatistics.g.h"
std::wstring TempFile();
extern wchar_t MainDirectory[MAX_PATH];

namespace winrt::WuiFET::implementation
{
    struct ResultsStatistics : ResultsStatisticsT<ResultsStatistics>
    {
        ResultsStatistics()
        {
            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
        }

        void OnNavigatedTo(winrt::Microsoft::UI::Xaml::Navigation::NavigationEventArgs const& e)
        {
            if (e.Parameter())
            {
                PROJECT_AND_RESULT* param = (PROJECT_AND_RESULT*)e.Parameter().as<long long>();
                _ResultX = param->result;
                _ResultP = param->project;
                delete param;
            }
        }

        static winrt::hstring txt(long jx)
        {
            return s(jx);
        }


        winrt::event_token PropertyChanged(winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler)
        {
            return m_propertyChanged.add(handler);
        }
        void PropertyChanged(winrt::event_token const& token) noexcept
        {
            m_propertyChanged.remove(token);
        }
        winrt::event<Microsoft::UI::Xaml::Data::PropertyChangedEventHandler> m_propertyChanged;

        void Refresh(const wchar_t* s)
        {
            m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs{ s });
        }

        void Refresh()
        {
            m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs{ L"" });
        }

        XML3::XMLElement* _ResultX = 0;
        long long ResultX()
        {
            return (long long)_ResultX;
        }
        void ResultX(long long x)
        {
            _ResultX = (XML3::XMLElement*)x;
        }
        PROJECT* _ResultP = 0;
        long long ResultP()
        {
            return (long long)_ResultP;
        }
        void ResultP(long long x)
        {
            _ResultP = (PROJECT*)x;
        }


        void ChartShow(std::string html)
        {
            const char* hdr = R"(
<html>

<script src="chart.js"></script>
   <style>
    html, body {
      margin: 0;
      height: 100%;
      background: #f8f9fa;
      font-family: Arial, sans-serif;
      display: flex;
      flex-direction: column;
      align-items: center;
    }
    h2 {
      color: #333;
      margin: 20px 0;
    }
    #chart-wrapper {
      width: 90vw;
      height: 80vh;
      overflow-x: auto;     /*  enables horizontal scroll */
      overflow-y: hidden;
      border: 1px solid #ccc;
      background: #fff;
      border-radius: 10px;
      box-shadow: 0 0 10px rgba(0,0,0,0.1);
    }
    #chart-container {
      width: 2000px;        /*  make this larger than viewport */
      height: 100%;
    }
    canvas {
      width: 100% !important;
      height: 100% !important;
    }
  </style>

 <canvas id="chart" ></canvas>

)";
            auto tf = TempFile();
            SHCreateDirectory(0, tf.c_str());
            if (1)
            {
                auto t2 = tf;
                t2 += L"\\chart.js";
                std::wstring cs = MainDirectory;
                cs += L"\\chart.js";
                CopyFile(cs.c_str(), t2.c_str(), FALSE);
            }
            auto t1 = tf;
            t1 += L"\\index.html";
            DeleteFile(t1.c_str());
            std::vector<char> d(strlen(hdr) + html.length());
            memcpy(d.data(), hdr, strlen(hdr));
            memcpy(d.data() + strlen(hdr), html.data(), html.length());
            PutFile(t1.c_str(), d);
            auto wb = webView();
            if (wb)
                wb.Source(winrt::Windows::Foundation::Uri{ t1.c_str() });
        }



        void MaxSpan(auto, auto)
        {
            std::string w;
            w += R"(
  <script>
    // Get canvas context
    const ctx = document.getElementById('chart').getContext('2d');

    // Create the bar chart
    new Chart(ctx, {
      type: 'bar',
      data: {
        labels: [)";


            struct T
            {
                std::string name;
                ystring u;
                int Span = 0;
            };
            std::vector<T> Teachers;

            auto& TeachersList = (*_ResultX)["Teachers_Timetable"];
            auto daysx = _ResultP->DaysNames();
            auto hoursx = _ResultP->HoursNames();
            for (auto& t : TeachersList)
            {
                T tt;
                tt.u = t.vv("name").GetValue();

				auto te = _ResultP->FindSubElementByName(&_ResultP->x->GetRootElement()["Teachers_List"], tt.u.c_str());
				if (te)
                    tt.name = DisplayName(*te);


                std::map<int, int> d1;

                for (auto& t2 : t)
                {
                    if (t2.GetElementName() != "Day")
						continue;

                    // find day index
					int day_index = -1;
                    for (size_t di = 0; di < daysx.size(); di++)
                    {
                        if (daysx[di] == ystring(t2.vv("name").GetValue().c_str()))
                        {
                            day_index = (int)di;
                            break;
                        }
					}
                    if (day_index == -1)
						continue;

                    int min_hour = -1;
                    int max_hour = -1;
                    for (auto& t3 : t2)
                    {
                        // Hours
                        if (t3.GetElementName() != "Hour")
							continue;


                        if (t3.GetChildrenNum() == 0)
                            continue;

                        // hour index
						int hour_index = -1;
                        for (size_t hi = 0; hi < hoursx.size(); hi++)
                        {
                            if (hoursx[hi] == ystring(t3.vv("name").GetValue().c_str()))
                            {
                                hour_index = (int)hi;
                                break;
                            }
                        }
						if (hour_index == -1)
							continue;

                        if (min_hour == -1 || hour_index < min_hour)
                            min_hour = hour_index;
                        if (max_hour == -1 || hour_index > max_hour)
							max_hour = hour_index;


                    }
					d1[day_index] = (max_hour - min_hour) + 1;
                }

                for (auto& dd : d1)
                {
                    tt.Span = std::max(tt.Span,dd.second);
				}
                




                w += "'";
                w += tt.name;
                w += "',";
                Teachers.push_back(tt);

            }
            w += R"(],
datasets: [{
          label: '',
          data: [)";

            for (auto& t : Teachers)
            {
                w += std::to_string(t.Span);
                w += ",";
            }

            w += R"(],
          backgroundColor: [)";

            for (auto& t : Teachers)
            {
                t;
                w += "'rgba(0, 192, 0, 0.6)',";
            }


            w += R"(],
          borderColor: 'rgba(0,0,0,0.2)',
          borderWidth: 1
        }]
      },
      options: {
        responsive: false,
        plugins: {
          legend: {
            display: true,
            labels: { color: '#333' }
          },
          title: {
            display: true,
            text: '',
            color: '#333',
            font: { size: 20 }
          }
        },
        scales: {
          y: {
            beginAtZero: true,
            ticks: { color: '#555' },
            grid: { color: '#eee' }
          },
          x: {
            ticks: { color: '#555' },
            grid: { color: '#eee' }
          }
        }
      }
    });
  </script>)";

            ChartShow(w);
        }



        void TotalGaps(auto, auto)
        {
            std::string w;
            w += R"(
  <script>
    // Get canvas context
    const ctx = document.getElementById('chart').getContext('2d');

    // Create the bar chart
    new Chart(ctx, {
      type: 'bar',
      data: {
        labels: [)";


            struct T
            {
                std::string name;
                ystring u;
                int Gaps = 0;
            };
            std::vector<T> Teachers;

            auto& TeachersList = (*_ResultX)["Teachers_Timetable"];
            auto daysx = _ResultP->DaysNames();
            auto hoursx = _ResultP->HoursNames();
            for (auto& t : TeachersList)
            {
                T tt;
                tt.u = t.vv("name").GetValue();

                auto te = _ResultP->FindSubElementByName(&_ResultP->x->GetRootElement()["Teachers_List"], tt.u.c_str());
                if (te)
                    tt.name = DisplayName(*te);


                for (auto& t2 : t)
                {
                    if (t2.GetElementName() != "Day")
                        continue;

                    // find day index
                    int day_index = -1;
                    for (size_t di = 0; di < daysx.size(); di++)
                    {
                        if (daysx[di] == ystring(t2.vv("name").GetValue().c_str()))
                        {
                            day_index = (int)di;
                            break;
                        }
                    }
                    if (day_index == -1)
                        continue;

                    int Started = 0;
                    int SureGaps = 0;
                    int PendingGaps = 0;
                    for (auto& t3 : t2)
                    {
                        // Hours
                        if (t3.GetElementName() != "Hour")
                            continue;
                        if (t3.GetChildrenNum() == 0)
                        {
                            if (Started)
                                PendingGaps++;
                            continue;
                        }
						SureGaps += PendingGaps;
                        PendingGaps = 0;
                        Started = 1;
                    }
                    tt.Gaps += SureGaps;
                }


                w += "'";
                w += tt.name;
                w += "',";
                Teachers.push_back(tt);

            }
            w += R"(],
datasets: [{
          label: '',
          data: [)";

            for (auto& t : Teachers)
            {
                w += std::to_string(t.Gaps);
                w += ",";
            }

            w += R"(],
          backgroundColor: [)";

            for (auto& t : Teachers)
            {
                t;
                w += "'rgba(0, 192, 0, 0.6)',";
            }


            w += R"(],
          borderColor: 'rgba(0,0,0,0.2)',
          borderWidth: 1
        }]
      },
      options: {
        responsive: false,
        plugins: {
          legend: {
            display: true,
            labels: { color: '#333' }
          },
          title: {
            display: true,
            text: '',
            color: '#333',
            font: { size: 20 }
          }
        },
        scales: {
          y: {
            beginAtZero: true,
            ticks: { color: '#555' },
            grid: { color: '#eee' }
          },
          x: {
            ticks: { color: '#555' },
            grid: { color: '#eee' }
          }
        }
      }
    });
  </script>)";

            ChartShow(w);
        }
    };
}

namespace winrt::WuiFET::factory_implementation
{
    struct ResultsStatistics : ResultsStatisticsT<ResultsStatistics, implementation::ResultsStatistics>
    {
    };
}

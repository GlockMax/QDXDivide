#include "stdafx.h"
#include "QDXDivide.h"
#include <fstream>

QDXDivide::QDXDivide(MBody* body) : QDivideBase()
{
	pb = dynamic_cast<DXSubModel5X*>(body);
	if (pb == nullptr) assert("Неудачный dynamic_cast");
}


QDXDivide::~QDXDivide()
{
    if (copy != nullptr) delete copy;
}


// Пометить отдельные тела в модели
void QDXDivide::MarkBodies() {

    // Очередь из отрезков декселей
    std::list<QueueMember> q;

    // Массив z-декселей
    const DXDirection5X& dir_z = pb->GetDirection(0);

    // Доступ к точкам отрезков z-декселей
    DXMemoryPtOnePart& act_mem_z = pb->GetMemory(0);
    // NEW Зануляем метки
    act_mem_z.InitTags();

    // Шаг z-декселей по оси x
    double step_x = dir_z.GetStep0();

    // Шаг z-декселей по оси y
    double step_y = dir_z.GetStep1();

    // Шаг x/y-декселей по оси z
    double step_z = (pb->GetDirection(1)).GetStep0();

    double steps[3] = { step_z, step_y, step_x };
    bool inits[3] = { true, false, false }; // Проинициализировали/обнулили ли мы метки тела?

    // Счётчик тел, он же - их метка
    BYTE count = 1;

    // Цикл по z-декселям
    for (int x0 = 0; x0 < dir_z.GetN0(); ++x0) {
        for (int y0 = 0; y0 < dir_z.GetN1(); ++y0) {

            DXMemID start = dir_z.GetDexelStartPoint(x0, y0); // Начальная точка в ячейке декселя

            if (start == DXP_END)
                continue; // Пустой дексель, переходим к следующему

            for (DXMemID next = start; next != DXP_END; )
            {
                DXMemID b = act_mem_z.GetAtNext(next);
                // Точки отрезка - next и b

                // Получаем метки точек
                BYTE id_s1 = act_mem_z.GetAtTag(next);
                BYTE id_b1 = act_mem_z.GetAtTag(b);

                // Если отрезок не отмечен
                if ((id_s1 == 0) || (id_b1 == 0)) {

                    id_s1 = count;
                    id_b1 = count;

                    act_mem_z.SetAtTag(next, id_s1);
                    act_mem_z.SetAtTag(b, id_b1);


                    // Заталкиваем в очередь только начало отрезка (конец мы всегда можем получить)
                    q.emplace_back(next, DexelSectionType::Z_D, x0, y0);

                    // ------------------------------------------------------------------------
                    // Запускаем цикл обработки очереди отрезков
                    while (!q.empty()) {

                        QueueMember u = q.front();
                        q.pop_front(); // Удаляем из очереди обрабатываемый отрезок декселя

                        // Определяем эту переменную для компактного кода
                        int c = u.t;

                        DXMemoryPtOnePart& act_mem_p = pb->GetMemory(c);
                        // NEW Проверяем инициализацию меток
                        if (!inits[c]) {
                            act_mem_p.InitTags();
                            inits[c] = !inits[c];
                        }
                        // end of NEW

                        DX_DEPTH p1 = act_mem_p.GetAtZ((u.p)); // Первая точка отрезка
                        DXMemID u1 = act_mem_p.GetAtNext((u.p)); // Вторая точка отрезка, её тоже нужно отметить

                        DX_DEPTH p2 = act_mem_p.GetAtZ(u1); // Вторая точка отрезка

                        // Дексели по направлению n1
                        const DXDirection5X& dir_n1 = pb->GetDirection((c + 1) % 3);
                        // Дексели по направлению n0
                        const DXDirection5X& dir_n0 = pb->GetDirection((c + 2) % 3);

                        // Доступ к точкам отрезков декселей по направлению n0
                        DXMemoryPtOnePart& act_mem_n1 = pb->GetMemory((c + 1) % 3);

                        if (!inits[(c + 1) % 3]) {
                            act_mem_n1.InitTags();
                            inits[c] = !inits[(c + 1) % 3];
                        }

                        // Доступ к точкам отрезков декселей по направлению n1
                        DXMemoryPtOnePart& act_mem_n0 = pb->GetMemory((c + 2) % 3);

                        if (!inits[(c + 2) % 3]) {
                            act_mem_n1.InitTags();
                            inits[c] = !inits[(c + 2) % 3];
                        }

                        // ------------------------------------------------------------------------
                        // Обработка декселей по направлению n1
                        for (int p_i = ceil(p1 / steps[c]); p_i <= (floor(p2 / steps[c])); ++p_i) {
                            DXMemID start_n1 = dir_n1.GetDexelStartPoint(p_i, u.i_n0);

                            if (start_n1 == DXP_END) continue;

                            for (DXMemID next_n1 = start_n1; next_n1 != DXP_END; ) {
                                DXMemID b_n1 = act_mem_n1.GetAtNext(next_n1);
                                // Точки отрезка - next_n1 и b_n1

                                
                                DX_DEPTH p_next_n1 = act_mem_n1.GetAtZ(next_n1);
                                
                                DX_DEPTH p_b_n1 = act_mem_n1.GetAtZ(b_n1);

                                BYTE id_next_n11 = act_mem_n1.GetAtTag(next_n1);
                                BYTE id_b_n11 = act_mem_n1.GetAtTag(b_n1);

                                // Если отрезок рассматриваемого направления содержит координату отрезка декселя U
                                if ((u.i_n1 * steps[(c + 1) % 3] >= p_next_n1) && (u.i_n1 * steps[(c + 1) % 3] <= p_b_n1)) {
                                    // Если отрезок не отмечен
                                    if ((id_next_n11 == 0) || (id_b_n11 == 0)) {
                                        
                                        id_next_n11 = count;
                                        id_b_n11 = count;

                                        act_mem_n1.SetAtTag(next_n1, id_next_n11);
                                        act_mem_n1.SetAtTag(b_n1, id_b_n11);

                                        q.emplace_back(next_n1, DexelSectionType((c + 1) % 3), p_i, u.i_n0);
                                        // Обрабатывать отрезки дальше нет смысла
                                        break;
                                    }
                                }
                                next_n1 = act_mem_n1.GetAtNext(b_n1);
                            }
                        }

                        // ------------------------------------------------------------------------
                        // Обработка декселей по направлению n0
                        for (int p_i = ceil(p1 / steps[c]); p_i <= (floor(p2 / steps[c])); ++p_i) {
                            DXMemID start_n0 = dir_n0.GetDexelStartPoint(u.i_n1, p_i);

                            if (start_n0 == DXP_END) continue;
                            for (DXMemID next_n0 = start_n0; next_n0 != DXP_END; ) {

                                DXMemID b_n0 = act_mem_n0.GetAtNext(next_n0);
                                // Точки отрезка - next_n0 и b_n0
                                
                                DX_DEPTH p_next_n0 = act_mem_n0.GetAtZ(next_n0);
                                DX_DEPTH p_b_n0 = act_mem_n0.GetAtZ(b_n0);

                                BYTE id_next_n01 = act_mem_n0.GetAtTag(next_n0);
                                BYTE id_b_n01 = act_mem_n0.GetAtTag(b_n0);

                                // Если отрезок рассматриваемого направления содержит координату отрезка декселя
                                if ((u.i_n0 * steps[(c + 2) % 3] >= p_next_n0) && (u.i_n0 * steps[(c + 2) % 3] <= p_b_n0)) {
                                    // Если отрезок не отмечен
                                    if ((id_next_n01 == 0) || (id_b_n01 == 0)) {
                                        
                                        id_next_n01 = count;
                                        id_b_n01 = count;

                                        act_mem_n0.SetAtTag(next_n0, id_next_n01);
                                        act_mem_n0.SetAtTag(b_n0, id_b_n01);

                                        q.emplace_back(next_n0, DexelSectionType((c + 2) % 3), u.i_n1, p_i);

                                        // Обрабатывать отрезки дальше нет смысла
                                        break;
                                    }
                                }
                                next_n0 = act_mem_n0.GetAtNext(b_n0);
                            }
                        }
                    }
                    // Увеличиваем счётчик тел
                    count < 255 ? count++ : count;
                }
                // Следующий отрезок
                next = act_mem_z.GetAtNext(b);
            }
        }
    }

    // Вывод в файлик для дебага
    std::ofstream out_file("Отмеченные Z-дексели.txt", std::ios_base::out);
    for (int x0 = 0; x0 < dir_z.GetN0(); ++x0) {
        for (int y0 = 0; y0 < dir_z.GetN1(); ++y0) {

            DXMemID start = dir_z.GetDexelStartPoint(x0, y0); // Начальная точка в ячейке декселя

            if (start == DXP_END) {
                out_file << ' ';
                continue; // Пустой дексель, переходим к следующему
            }

            out_file << ((int) act_mem_z.GetAtTag(start));
            //out_file << act_mem_z.GetAtID(start).Cadr.Cadr;
        }
        out_file << std::endl;
    }
    out_file.close();
}


void QDXDivide::DelByPoint(BYTE tag, bool delExceptThis) {
    //DXSubModel5X* pb = body;

    for (int c = 0; c < 3; c++) {

        DXDirection5X& dir_d = const_cast<DXDirection5X&>(pb->GetDirection(c));
        DXMemoryPtOnePart& act_mem_d = pb->GetMemory(c);

        for (int i_n0 = 0; i_n0 < dir_d.GetN0(); ++i_n0) {
            for (int i_n1 = 0; i_n1 < dir_d.GetN1(); ++i_n1) {

                DXMemID start = dir_d.GetDexelStartPoint(i_n0, i_n1); // Начальная точка в ячейке декселя

                if (start == DXP_END)
                    continue; // Пустой дексель, переходим к следующему

                DXMemID prev_end = DXP_END;
                /*
                                              Текущий отрезок
                  ... *------------*        *-----------------*   *-------------* ...
                                   ^        ^                 ^   ^
                            prev_end     next                 b   t
                */
                for (DXMemID next = start; next != DXP_END; )
                {
                    DXMemID b = act_mem_d.GetAtNext(next);
                    // Точки отрезка - next и b

                    // Начальная точка следующего отрезка
                    DXMemID t = act_mem_d.GetAtNext(b);

                    // Получаем ID точек
                    BYTE tag_next = act_mem_d.GetAtTag(next);
                    BYTE tag_b = act_mem_d.GetAtTag(b);

                    // Если обе точки отрезка не отмечены единицей
                    if ((tag_next != tag) || (tag_b != tag)) {
                        // Если начало отрезка - начало самого декселя:
                        if (next == start) {
                            // Устанавливаем начало декселя в начальную точку следующего отрезка (даже если он - DXP_END)
                            dir_d.SetDexelStartPoint(i_n0, i_n1, t);
                            // Обновляем начало декселя
                            start = t;
                            //next = t;
                            //continue;
                        }
                        // Если начало текущего отрезка - не начало декселя
                        else {
                            // Связываем конец предыдущего отрезка с началом следующего (даже если он - DXP_END)
                            if (prev_end == DXP_END) {
                                assert("конец предыдущего отрезка не должен быть DXP_END");
                            }
                            act_mem_d.SetAtNext(prev_end, t);
                            //next = t;


                        }

                        act_mem_d.RemoveAt(next);
                        act_mem_d.RemoveAt(b);
                        next = t;
                        continue;
                    }

                    next = t;
                    prev_end = b; // ?
                }
            }
        }

    }
}


bool QDXDivide::GetNearestPoint(BPoint p, BPoint s, BPoint* result, int* c, int* face) {
    BPoint A = pb->GetMin();
    BPoint B = pb->GetMax();

    double x = A.GetX() - 1;
    double y = A.GetY() - 1;
    double z = A.GetZ() - 1;

    // =========================================================================================
    
    // Проверка скалярного произведения вектора s с (0; 0; 1) (нормаль для плоскости z = z_b)
    if (s.GetZ() < 0) {
        // плоскость z = z_b
        (*face) = -1;
        x = s.GetX() * (B.GetZ() - p.GetZ()) / s.GetZ() + p.GetX();
        y = s.GetY() * (B.GetZ() - p.GetZ()) / s.GetZ() + p.GetY();
        z = B.GetZ();
    }
    // Если скалярное произведение имеет другой знак, значит вектор "входит" в плоскость z = z_a
    else if (s.GetZ() > 0) {
        (*face) = 1;
        x = s.GetX() * (A.GetZ() - p.GetZ()) / s.GetZ() + p.GetX();
        y = s.GetY() * (A.GetZ() - p.GetZ()) / s.GetZ() + p.GetY();
        z = A.GetZ();
    }
    if ( (A.GetX() < x) && (x < B.GetX()) && (A.GetY() < y) && (y < B.GetY()) ) {
        (*result) = BPoint(x, y, z);
        (*c) = 0;
        return true;
    }

    // =========================================================================================

    if (s.GetY() < 0) {
        // плоскость y = y_b
        (*face) = -1;
        x = s.GetX() * (B.GetY() - p.GetY()) / s.GetY() + p.GetX();
        y = B.GetY();
        z = s.GetZ() * (B.GetY() - p.GetY()) / s.GetY() + p.GetZ();
    }
    else if (s.GetY() > 0) {
        (*face) = 1;
        x = s.GetX() * (A.GetY() - p.GetY()) / s.GetY() + p.GetX();
        y = A.GetY();
        z = s.GetZ() * (A.GetY() - p.GetY()) / s.GetY() + p.GetZ();
    }
    if ((A.GetX() < x) && (x < B.GetX()) && (A.GetZ() < z) && (z < B.GetZ())) {
        (*result) = BPoint(x, y, z);
        (*c) = 1;
        return true;
    }

    // =========================================================================================

    if (s.GetX() < 0) {
        // плоскость x = x_b
        (*face) = -1;
        x = B.GetX();
        y = s.GetY() * (B.GetX() - p.GetX()) / s.GetX() + p.GetY();
        z = s.GetZ() * (B.GetX() - p.GetX()) / s.GetX() + p.GetZ();
    }
    else if (s.GetX() > 0) {
        (*face) = 1;
        x = A.GetX();
        y = s.GetY() * (A.GetX() - p.GetX()) / s.GetX() + p.GetY();
        z = s.GetZ() * (A.GetX() - p.GetX()) / s.GetX() + p.GetZ();
    }
    if ((A.GetZ() < z) && (z < B.GetZ()) && (A.GetY() < y) && (y < B.GetY())) {
        (*result) = BPoint(x, y, z);
        (*c) = 2;
        return true;
    }

    return false;
}


void QDXDivide::SetPoint(BPoint ipoint, BPoint iview)
{
	BPoint p(ipoint * CorrMatr.invr()); // точка в пространстве, куда приложен вектр взгляда
	BPoint s(iview * CorrMatr.invr()); // вектор взгляда на модель

    // Если копии тела нет, то создаём её
    if (copy == nullptr) {
        MarkBodies();
        pb->CreateFullCopy(copy);
    }
    // Если же копия есть, то и пометки в ней тоже есть

    double step_x = (pb->GetDirection(0)).GetStep0();

    // Шаг z-декселей по оси y
    double step_y = (pb->GetDirection(0)).GetStep1();

    // Шаг x/y-декселей по оси z
    double step_z = (pb->GetDirection(1)).GetStep0();

    double steps[3] = { step_z, step_y, step_x };
    double ns[3] = { (pb->GetDirection(1)).GetN0(), (pb->GetDirection(0)).GetN1(), (pb->GetDirection(0)).GetN0()};

    int c = -1; // Номер оси, перпендикулярно которой расположена грань ограничивающего параллепипеда,
                // в который мы попали
    BPoint pn;  // Точка, в которую мы попали
    int face = 0; // 1 - грань содержит минимальную точку, -1 - грань содержит максимальную точку 

    if (GetNearestPoint(p, s, &pn, &c, &face)) {
        BPoint local_coords = pn - pb->GetMin(); // Координаты точки в системе координат ограничивающего параллелепипеда

        DXDirection5X& dir_d = const_cast<DXDirection5X&>(pb->GetDirection(c));
        DXMemoryPtOnePart& act_mem_d = pb->GetMemory(c);

        int an_c = (3 - c - 1) % 3; // Индексы декселей не совпадают с индексами точек, нужно их вот так преобразовать

        double t = (local_coords[an_c] - p[an_c]) / s[an_c];
        BYTE res = 0;

        for (; (0 <= (s[an_c]*t+p[an_c])) && ((s[an_c] * t + p[an_c]) <= ns[c] * steps[c]); t += face * steps[c] / s[an_c]) {
            double c_n0 = (t*s[(an_c - 2) % 3] + p[(an_c - 2) % 3]) / steps[(c + 2) % 3];
            double c_n1 = (t * s[(an_c - 1) % 3] + p[(an_c - 1) % 3])/steps[(c+1)%3];

            // Координаты ещё нужно округлить и вычислить их индексы

            int i_n0_cl = ceil(c_n0);
            int i_n1_cl = ceil(c_n1);
            int i_n0_fl = floor(c_n0);
            int i_n1_fl = floor(c_n1);

            DXMemID start = dir_d.GetDexelStartPoint(i_n0_cl, i_n1_cl);

            for (DXMemID next = start; next != DXP_END; )
            {
                DXMemID b = act_mem_d.GetAtNext(next);
                DX_DEPTH next_d = act_mem_d.GetAtZ(next);
                DX_DEPTH b_d = act_mem_d.GetAtZ(b);

                if ((next_d < (s[an_c] * t + p[an_c])) && ((s[an_c] * t + p[an_c]) < b_d))
                    res = act_mem_d.GetAtTag(next);

                next = act_mem_d.GetAtNext(b);
            }

            start = dir_d.GetDexelStartPoint(i_n0_cl, i_n1_fl);

            for (DXMemID next = start; next != DXP_END; )
            {
                DXMemID b = act_mem_d.GetAtNext(next);

                DX_DEPTH next_d = act_mem_d.GetAtZ(next);
                DX_DEPTH b_d = act_mem_d.GetAtZ(b);

                if ((next_d < (s[an_c] * t + p[an_c])) && ((s[an_c] * t + p[an_c]) < b_d))
                    res = act_mem_d.GetAtTag(next);

                next = act_mem_d.GetAtNext(b);
            }

            start = dir_d.GetDexelStartPoint(i_n0_fl, i_n1_cl);

            for (DXMemID next = start; next != DXP_END; )
            {
                DXMemID b = act_mem_d.GetAtNext(next);

                DX_DEPTH next_d = act_mem_d.GetAtZ(next);
                DX_DEPTH b_d = act_mem_d.GetAtZ(b);

                if ((next_d < (s[an_c] * t + p[an_c])) && ((s[an_c] * t + p[an_c]) < b_d))
                    res = act_mem_d.GetAtTag(next);

                next = act_mem_d.GetAtNext(b);
            }

            start = dir_d.GetDexelStartPoint(i_n0_fl, i_n1_fl);

            for (DXMemID next = start; next != DXP_END; )
            {
                DXMemID b = act_mem_d.GetAtNext(next);

                DX_DEPTH next_d = act_mem_d.GetAtZ(next);
                DX_DEPTH b_d = act_mem_d.GetAtZ(b);

                if ((next_d < (s[an_c] * t + p[an_c])) && ((s[an_c] * t + p[an_c]) < b_d))
                    res = act_mem_d.GetAtTag(next);

                next = act_mem_d.GetAtNext(b);
            }
        
        }

        /*for (int i_n0 = 0; i_n0 < dir_d.GetN0(); ++i_n0) {
            for (int i_n1 = 0; i_n1 < dir_d.GetN1(); ++i_n1) {

                DXMemID start = dir_d.GetDexelStartPoint(i_n0, i_n1); // Начальная точка в ячейке декселя

                if (start == DXP_END)
                    continue;

                for (DXMemID next = start; next != DXP_END; )
                {
                    DXMemID b = act_mem_d.GetAtNext(next);

                    

                    next = act_mem_d.GetAtNext(b);
                }

            }
        }*/

        res;
    }



	// DXSubModel5X::GetMin
	// find and delete object which was clicked
	switch (mode)
	{
	case REMOVE_CURRENT:
		break;
	case REMOVE_OTHERS:
		break;
	default:
		break;
	}
}

void QDXDivide::Revert()
{
}


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
}


// Пометить отдельные тела в модели
void QDXDivide::MarkBodies() {
    // Очередь из отрезков декселей
    std::list<QueueMember> q;

    // Массив z-декселей
    const DXDirection5X& dir_z = pb->GetDirection(0);

    // Доступ к точкам отрезков z-декселей
    DXMemoryPtOnePart& act_mem_z = pb->GetMemory(0);

    // Шаг z-декселей по оси x
    double step_x = dir_z.GetStep0();

    // Шаг z-декселей по оси y
    double step_y = dir_z.GetStep1();

    // Шаг x/y-декселей по оси z
    double step_z = (pb->GetDirection(1)).GetStep0();

    double steps[3] = { step_z, step_y, step_x };

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

                // Получаем ID точек
                DX_ID id_s = act_mem_z.GetAtID(next);
                DX_ID id_b = act_mem_z.GetAtID(b);
                BYTE id_s1 = act_mem_z.GetAtTag(next);
                BYTE id_b1 = act_mem_z.GetAtTag(b);

                // Если отрезок не отмечен
                //if ((id_s.Cadr.Cadr == 0) || (id_b.Cadr.Cadr == 0)) {
                if ((id_s1 == 0) || (id_b1 == 0)) {

                    id_s.Cadr.Cadr = count;
                    id_b.Cadr.Cadr = count;

                    act_mem_z.SetAtID(next, id_s);
                    act_mem_z.SetAtID(b, id_b);

                    id_s1 = count;
                    id_b1 = count;

                    act_mem_z.SetAtTag(next, id_s1);
                    act_mem_z.SetAtTag(b, id_b1);
                    int hui = 1;


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

                        DX_DEPTH p1 = act_mem_p.GetAtZ((u.p)); // Первая точка отрезка
                        DXMemID u1 = act_mem_p.GetAtNext((u.p)); // Вторая точка отрезка, её тоже нужно отметить

                        DX_DEPTH p2 = act_mem_p.GetAtZ(u1); // Вторая точка отрезка

                        BYTE deb = act_mem_p.GetAtTag(u.p);

                        // Дексели по направлению n1
                        const DXDirection5X& dir_n1 = pb->GetDirection((c + 1) % 3);
                        // Дексели по направлению n0
                        const DXDirection5X& dir_n0 = pb->GetDirection((c + 2) % 3);

                        // Доступ к точкам отрезков декселей по направлению n0
                        DXMemoryPtOnePart& act_mem_n1 = pb->GetMemory((c + 1) % 3);
                        // Доступ к точкам отрезков декселей по направлению n1
                        DXMemoryPtOnePart& act_mem_n0 = pb->GetMemory((c + 2) % 3);

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

                                DX_ID id_next_n1 = act_mem_n1.GetAtID(next_n1);
                                DX_ID id_b_n1 = act_mem_n1.GetAtID(b_n1);
                                // NEW
                                BYTE id_next_n11 = act_mem_n1.GetAtTag(next_n1);
                                BYTE id_b_n11 = act_mem_n1.GetAtTag(b_n1);

                                // Если отрезок рассматриваемого направления содержит координату отрезка декселя U
                                if ((u.i_n1 * steps[(c + 1) % 3] >= p_next_n1) && (u.i_n1 * steps[(c + 1) % 3] <= p_b_n1)) {
                                    // Если отрезок не отмечен
                                    if ((id_next_n11 == 0) || (id_b_n11 == 0)) {
                                    //if ((id_next_n1.Cadr.Cadr == 0) || (id_b_n1.Cadr.Cadr == 0)) {

                                        id_next_n1.Cadr.Cadr = count;
                                        id_b_n1.Cadr.Cadr = count;

                                        act_mem_n1.SetAtID(next_n1, id_next_n1);
                                        act_mem_n1.SetAtID(b_n1, id_b_n1);
                                        
                                        id_next_n11 = count;
                                        id_b_n11 = count;

                                        act_mem_n1.SetAtTag(next_n1, id_next_n11);
                                        act_mem_n1.SetAtTag(b_n1, id_b_n11);

                                        //q.push_back(QueueMember(next_n1, DexelSectionType((c + 1) % 3), p_i, u.i_n0));
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

                                DX_ID id_next_n0 = act_mem_n0.GetAtID(next_n0);
                                DX_ID id_b_n0 = act_mem_n0.GetAtID(b_n0);
                                // NEW
                                BYTE id_next_n01 = act_mem_n1.GetAtTag(next_n0);
                                BYTE id_b_n01 = act_mem_n1.GetAtTag(b_n0);

                                // Если отрезок рассматриваемого направления содержит координату отрезка декселя
                                if ((u.i_n0 * steps[(c + 2) % 3] >= p_next_n0) && (u.i_n0 * steps[(c + 2) % 3] <= p_b_n0)) {
                                    // Если отрезок не отмечен
                                    if ((id_next_n01 == 0) || (id_b_n01 == 0)) {
                                    //if ((id_next_n0.Cadr.Cadr == 0) || (id_b_n0.Cadr.Cadr == 0)) {

                                        id_next_n0.Cadr.Cadr = count;
                                        id_b_n0.Cadr.Cadr = count;

                                        act_mem_n0.SetAtID(next_n0, id_next_n0);
                                        act_mem_n0.SetAtID(b_n0, id_b_n0);

                                        
                                        id_next_n01 = count;
                                        id_b_n01 = count;

                                        act_mem_n0.SetAtTag(next_n0, id_next_n01);
                                        act_mem_n0.SetAtTag(b_n0, id_b_n01);

                                        //q.push_back(QueueMember(next_n0, DexelSectionType((c + 2) % 3), u.i_n1, p_i));
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


void QDXDivide::SetPoint(BPoint ipoint, BPoint iview)
{
	BPoint point(ipoint * CorrMatr.invr());
	BPoint view(iview * CorrMatr.invr());

    MarkBodies();

	pb->GetMin();



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


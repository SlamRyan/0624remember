#include "..\include\base_camera.h"
InsCamera::InsCamera()
{
}

InsCamera::~InsCamera()
{
}

InsResult InsCamera::ConnectCamera()
{
	return InsResult::INSCSTATUS_OK;
}
InsResult InsCamera::FindCamera()
{
	return InsResult::INSCSTATUS_OK;
}

InsResult InsCamera::GrabContinous()
{
	return InsResult::INSCSTATUS_OK;
}
InsResult InsCamera::GrabStop()
{
	return InsResult::INSCSTATUS_OK;
}
InsResult InsCamera::set_ins_width(int _index)
{
	return InsResult::INSCSTATUS_OK;
}
InsResult InsCamera::set_ins_height(int _ins_height)
{
	return InsResult::INSCSTATUS_OK;
}
int InsCamera::get_ins_width()
{
	return 0;
}
int InsCamera::get_ins_height()
{
	return 0;
}
InsResult InsCamera::set_ins_exposure(double _val)
{
	return InsResult::INSCSTATUS_OK;
}
double InsCamera::get_ins_exposure()
{
	return 0;
}
InsResult InsCamera::set_ins_digital_gain(double _val)
{
	return InsResult::INSCSTATUS_OK;
}
double InsCamera::get_ins_digital_gain()
{
	return 0;
}
package com.jason.lsearch.dispose;

import org.opencv.core.MatOfInt;
import org.opencv.core.MatOfInt4;
import org.opencv.core.MatOfPoint;
import org.opencv.core.MatOfPoint2f;
import org.opencv.core.Point;
import org.opencv.core.RotatedRect;
import org.opencv.imgproc.Imgproc;
import org.opencv.imgproc.Moments;

public class feature {

	private double length; // 轮廓周长
	private double area; // 轮廓面积
	private Point pt_c = new Point(); // 轮廓重心
	private double UR;// 重心到轮廓的平均距离
	private double PR;// 重心到轮廓点的均方差
	private double r_out = 0;// 外切圆半径
	private double r_in = 500;// 内接圆半径
	private double circle_area_out; // 最小外接圆面积
	private double circle_area_in; // 最大内切圆面积
	private double rect_w; // 最小外接矩形宽
	private double rect_h; // 最小外接矩形长
	private double rect_area; // 最小外接矩形面积
	private double hull_area; // 凸包面积
	private double hull_length; // 凸包周长
	private double long_axis; // 轮廓长轴
	private double short_axis; // 轮廓短轴
	private double l_c; // 重心到区域轮廓最短距离

	private double rectangularity; // 矩形度
	private double complexity; // 复杂度
	private double circularity; // 圆形度
	private double sphericity; // 球形度
	private double aspect_radio; // 横纵轴比
	private double a_convexity; // 面积凹凸度
	private double p_convexity; // 周长凹凸度
	private double eccentricity; // 偏心率
	private double lobation; // 叶状性

	private void contourF(MatOfPoint2f contour)// 获得轮廓周长和面积
	{
		length = Imgproc.arcLength(contour, true);
		area = Imgproc.contourArea(contour);

	}

	private void center(MatOfPoint2f contour)// 获得轮廓重心
	{
		Moments moments;
		double m00 = 0, m10, m01, inv_m00;
		int xc, yc; // 重心横纵坐标

		moments = Imgproc.moments(contour);

		m00 = moments.get_m00();
		m10 = moments.get_m10();
		m01 = moments.get_m01();
		moments.get_mu11();
		moments.get_mu20();
		moments.get_mu02();

		inv_m00 = 1. / m00;
		xc = (int) Math.round(m10 * inv_m00);
		yc = (int) Math.round(m01 * inv_m00);
		pt_c.x = xc;
		pt_c.y = yc;

		// 求区域到重心的最短距离与平均距离
		double sum = 0;
		int count = 0;
		l_c = Double.MAX_VALUE;
		Point contourPoint = new Point();

		for (int m = 0; m < contour.rows(); m++) {
			double[] present = contour.get(m, 0);
			contourPoint.x = present[0];
			contourPoint.y = present[1];
			double temp1 = Math.sqrt(Math.pow(contourPoint.x - pt_c.x, 2)
					+ Math.pow(contourPoint.y - pt_c.y, 2));
			;
			l_c = l_c < temp1 ? l_c : temp1;
			sum += temp1;
			count++;
		}
		UR = sum / count;

		// 求区域重心到轮廓点的均方差
		sum = 0;
		for (int m = 0; m < contour.rows(); m++) {
			double[] present = contour.get(m, 0);
			contourPoint.x = present[0];
			contourPoint.y = present[1];
			double temp1 = Math.sqrt(Math.pow(contourPoint.x - pt_c.x, 2)
					+ Math.pow(contourPoint.y - pt_c.y, 2));
			sum += Math.pow(temp1 - UR, 2);
		}
		PR = sum / count;
	}

	private void rect(MatOfPoint2f contour)// 获得最小外接矩形周长和面积
	{
		RotatedRect box = Imgproc.minAreaRect(contour);
		Point pt[] = new Point[4];
		box.points(pt);
		double l1 = Math.sqrt(Math.pow((pt[1].x - pt[0].x), 2)
				+ Math.pow((pt[1].y - pt[0].y), 2));
		double l2 = Math.sqrt(Math.pow((pt[2].x - pt[1].x), 2)
				+ Math.pow((pt[2].y - pt[1].y), 2));
		if (l1 < l2) {
			rect_w = l1;
			rect_h = l2;
		} else {
			rect_w = l2;
			rect_h = l1;
		}

		rect_area = rect_h * rect_w;

	}

	// 获得凸包的周长、面积
	// 获得最小外切圆周长和面积
	// 获得最大内结缘周长和面积
	// 获得轮廓长轴
	private void hull(MatOfPoint contour) {
		MatOfPoint2f contour2f = new MatOfPoint2f(contour.toArray());

		hull_area = 0.0;
		hull_length = 0.0;

		MatOfInt hull = new MatOfInt();
		Imgproc.convexHull(contour, hull, false);

		new Point();
		int[] presenti = new int[1];
		hull.get(0, 0, presenti);

		double[] presentd = new double[2];
		presentd = contour2f.get(presenti[0], 0);
		Point pt0 = new Point();
		Point pt1 = new Point();
		pt0.x = presentd[0];
		pt0.y = presentd[1];

		for (int i = 0; i < hull.cols(); ++i) {
			hull.get(i, 0, presenti);

			presentd = contour2f.get(presenti[0], 0);
			pt0.x = presentd[0];
			pt0.y = presentd[1];

			// 计算凸包面积
			// 叉乘算每个三角形的面积
			hull_area += Math
					.abs(((pt1.x - pt_c.x) * (pt0.y - pt_c.y) - (pt1.y - pt_c.y)
							* (pt0.x - pt_c.x)) / 2.0);

			// 计算凸包周长
			hull_length += Math.sqrt(Math.pow((pt0.x - pt1.x), 2)
					+ Math.pow((pt0.y - pt1.y), 2));

			pt0 = pt1;
		}

		r_out = 0.0;
		r_in = 500.0;

		// CvSeq *defect = cvConvexityDefects(cont, hull);
		MatOfInt4 convDef = new MatOfInt4();
		Imgproc.convexityDefects(contour, hull, convDef);
		Point s = new Point(); // point of the contour where the defect begins
		Point e = new Point(); // point of the contour where the defect ends
		Point d = new Point();
		int[] present4i = new int[4];

		for (int i = 0; i < convDef.rows(); ++i) {
			// Point s =new POint(); contour->at(convDef[i][0]); // point of the
			// contour where the defect begins
			// Point e = contour->at(convDef[i][1]); // point of the contour
			// where the defect ends
			// Point d = contour->at(convDef[i][2]); // the farthest from the
			// convex hull point within the defect
			convDef.get(i, 0, present4i);
			double[] present = contour2f.get(present4i[0], 0);
			s.x = present[0];
			s.y = present[1];
			present = contour2f.get(present4i[1], 0);
			e.x = present[0];
			e.y = present[1];
			present = contour2f.get(present4i[2], 0);
			d.x = present[0];
			d.y = present[1];

			double s_t = Math.sqrt(Math.pow((s.x - pt_c.x), 2)
					+ Math.pow((s.y - pt_c.y), 2));
			double e_t = Math.sqrt(Math.pow((e.x - pt_c.x), 2)
					+ Math.pow((e.y - pt_c.y), 2));
			double r_in_t = Math.sqrt(Math.pow((d.x - pt_c.x), 2)
					+ Math.pow((d.y - pt_c.y), 2));
			double r_out_t = s_t > e_t ? s_t : e_t;

			r_out = r_out > r_out_t ? r_out : r_out_t; // 最小外接圆半径为重心到轮廓上最远的点的距离
			r_in = r_in < r_in_t ? r_in : r_in_t; // 最大内切圆半径为重心到轮廓上最近的点的距离

		}

		circle_area_out = Math.PI * Math.pow(r_out, 2);
		circle_area_in = Math.PI * Math.pow(r_in, 2);

	}

	private void axis(MatOfPoint2f contour) {

		long_axis = 0;
		short_axis = 0;
		Point contourPoint1 = new Point(), contourPoint2 = new Point();
		Point pt_l1 = new Point(), pt_l2 = new Point(); // 长轴两端点
		new Point();

		for (int m = 0; m < contour.rows(); m++) {
			double[] present = contour.get(m, 0);
			contourPoint1.x = present[0];
			contourPoint1.y = present[1];
			for (int n = 0; n < contour.rows(); n++) {
				present = contour.get(n, 0);
				contourPoint2.x = present[0];
				contourPoint2.y = present[1];
				double p1_p2_t = Math.sqrt(Math.pow(contourPoint1.x
						- contourPoint2.x, 2)
						+ Math.pow(contourPoint1.y - contourPoint2.y, 2));
				if (p1_p2_t > long_axis) {
					long_axis = p1_p2_t;
					pt_l1 = contourPoint1;
					pt_l2 = contourPoint2;
				}
			}
		}

		if ((pt_l1.y - pt_l2.y) != 0 && (pt_l1.x - pt_l2.x) != 0) {
			double k_l = (pt_l1.y - pt_l2.y)
					/ (pt_l1.x - pt_l2.x);
			System.out.println("k_l:" + k_l);
			double k_s;

			for (int m = 0; m < contour.rows(); m++) {
				double[] present = contour.get(m, 0);
				contourPoint1.x = present[0];
				contourPoint1.y = present[1];
				for (int n = 0; n < contour.rows(); n++) {
					present = contour.get(n, 0);
					contourPoint2.x = present[0];
					contourPoint2.y = present[1];
					if (contourPoint1.x == contourPoint2.x)
						continue;
					k_s = (contourPoint1.y - contourPoint2.y)
							/ (contourPoint1.x - contourPoint2.x);
					if (contourPoint1.x - contourPoint2.x == 0)
						continue;

					double x;
					x = Math.abs(k_s * k_l + 1);
					if (x < 0.005) {
						double short_temp = Math
								.sqrt(Math.pow(contourPoint1.x
										- contourPoint2.x, 2)
										+ Math.pow(contourPoint1.y
												- contourPoint2.y, 2));
						if (short_temp > short_axis) {
							short_axis = short_temp;
						}
					}
				}
			}
		}
		else if ((pt_l1.x - pt_l2.x) == 0) {
			for (int m = 0; m < contour.rows(); m++) {
				double[] present = contour.get(m, 0);
				contourPoint1.x = present[0];
				contourPoint1.y = present[1];
				for (int n = 0; n < contour.rows(); n++) {
					present = contour.get(n, 0);
					contourPoint2.x = present[0];
					contourPoint2.y = present[1];
					if (contourPoint1.y != contourPoint2.y)
						continue;
					double short_temp = Math.sqrt(Math.pow(contourPoint1.x
							- contourPoint2.x, 2)
							+ Math.pow(contourPoint1.y - contourPoint2.y, 2));
					if (short_temp > short_axis) {
						short_axis = short_temp;
					}
				}
			}
		}
		else{
			for (int m = 0; m < contour.rows(); m++) {
				double[] present = contour.get(m, 0);
				contourPoint1.x = present[0];
				contourPoint1.y = present[1];
				for (int n = 0; n < contour.rows(); n++) {
					present = contour.get(n, 0);
					contourPoint2.x = present[0];
					contourPoint2.y = present[1];
					if (contourPoint1.x != contourPoint2.x)
						continue;
					double short_temp = Math.sqrt(Math.pow(contourPoint1.x
							- contourPoint2.x, 2)
							+ Math.pow(contourPoint1.y - contourPoint2.y, 2));
					if (short_temp > short_axis) {
						short_axis = short_temp;
					}
				}
			}
		}

	}

	public float[] getFeature(MatOfPoint contour) {
		float[] feature = new float[10];

		MatOfPoint2f contour2f = new MatOfPoint2f(contour.toArray());

		contourF(contour2f);
		center(contour2f);
		rect(contour2f);
		hull(contour);
		axis(contour2f);

		rectangularity = area / rect_area;
		complexity = Math.pow(length, 2) / area;
		circularity = UR / PR;
		sphericity = r_in / r_out;
		aspect_radio = rect_w / rect_h;
		a_convexity = area / hull_area;
		p_convexity = length / hull_length;
		eccentricity = long_axis / short_axis;
		lobation = l_c / short_axis;

		System.out.println("short_axis:" + short_axis);
		System.out.println("l_c:" + l_c);
		System.out.println("long_axis:" + long_axis);

		feature[0] = (float) rectangularity;
		feature[1] = (float) complexity;
		feature[2] = (float) circularity;
		feature[3] = (float) sphericity;
		feature[4] = (float) aspect_radio;
		feature[5] = (float) a_convexity;
		feature[6] = (float) p_convexity;
		feature[7] = (float) eccentricity;
		feature[8] = (float) lobation;

		return feature;

		/*
		 * fout << rectangularity << " "; fout << complexity << " "; fout <<
		 * circularity << " "; fout << sphericity << " "; fout << aspect_radio
		 * << " "; fout << a_convexity << " "; fout << p_convexity << " "; fout
		 * << eccentricity << " "; fout << lobation << " ";
		 */
	}
}
